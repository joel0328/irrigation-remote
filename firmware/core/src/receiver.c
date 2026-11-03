#include "irr/receiver.h"

#include <string.h>

#include "irr/version.h"

void irr_receiver_init(irr_receiver *r, const uint8_t key[IRR_AES_KEY], uint32_t net_id,
                       const irr_rt_config *cfg, uint32_t last_rx_counter,
                       uint32_t next_tx_counter)
{
    irr_aes128_init(&r->key, key);
    r->net_id = net_id;
    irr_replay_init(&r->replay, last_rx_counter);
    irr_tx_counter_init(&r->tx, next_tx_counter);
    irr_rt_init(&r->rt, cfg);
    r->last_reply_len = 0;
}

static irr_rx_action send(irr_receiver *r, uint8_t type, const uint8_t *payload,
                          uint8_t plen, uint8_t *reply, size_t *reply_len)
{
    irr_header h = {0};
    h.type = type;
    h.net_id = r->net_id;
    if (irr_tx_counter_take(&r->tx, &h.counter) != 0) {
        return IRR_RX_DROP;
    }
    if (irr_frame_encode(&r->key, &h, payload, plen, reply, reply_len) != IRR_FRAME_OK) {
        return IRR_RX_DROP;
    }
    memcpy(r->last_reply, reply, *reply_len);
    r->last_reply_len = *reply_len;
    return IRR_RX_REPLY;
}

static irr_rx_action ack(irr_receiver *r, uint32_t acked, uint8_t result, uint32_t now_ms,
                         uint8_t sense, uint8_t *reply, size_t *reply_len)
{
    uint32_t rem = irr_rt_remaining_s(&r->rt, now_ms);
    uint8_t p[9];
    irr_put_u32(&p[0], acked);
    p[4] = result;
    p[5] = r->rt.zone; /* pending zone shows during the dead time too */
    irr_put_u16(&p[6], (uint16_t)(rem > 0xffffu ? 0xffffu : rem));
    p[8] = sense;
    return send(r, IRR_MSG_ACK, p, sizeof p, reply, reply_len);
}

irr_rx_action irr_receiver_handle(irr_receiver *r, const uint8_t *buf, size_t len,
                                  uint32_t now_ms, uint8_t sense_flags, uint8_t *reply,
                                  size_t *reply_len)
{
    irr_header h;
    const uint8_t *pl;
    if (irr_frame_decode(&r->key, buf, len, &h, &pl) != IRR_FRAME_OK) {
        return IRR_RX_DROP;
    }
    if (h.net_id != r->net_id || (h.type & IRR_DIR_FROM_RX)) {
        return IRR_RX_DROP;
    }
    const uint8_t *tag = &buf[len - IRR_TAG_LEN];
    switch (irr_replay_check(&r->replay, h.counter, tag)) {
    case IRR_REPLAY_STALE:
        return IRR_RX_DROP;
    case IRR_REPLAY_DUPLICATE:
        if (r->last_reply_len == 0) {
            return IRR_RX_DROP;
        }
        memcpy(reply, r->last_reply, r->last_reply_len);
        *reply_len = r->last_reply_len;
        return IRR_RX_REPLY;
    case IRR_REPLAY_FRESH:
        break;
    }
    irr_replay_accept(&r->replay, h.counter, tag);
    irr_rt_tick(&r->rt, now_ms);

    switch (h.type) {
    case IRR_MSG_HELLO: {
        uint8_t p[5] = {IRR_FW_MAJOR, IRR_FW_MINOR, IRR_FW_PATCH, IRR_FW_DEV ? 1u : 0u,
                        r->rt.cfg.zone_count};
        return send(r, IRR_MSG_HELLO_REPLY, p, sizeof p, reply, reply_len);
    }
    case IRR_MSG_START: {
        if (h.len != 3) {
            return ack(r, h.counter, IRR_ACK_BAD_PAYLOAD, now_ms, sense_flags, reply, reply_len);
        }
        irr_rt_result res = irr_rt_start(&r->rt, pl[0], irr_get_u16(&pl[1]), now_ms);
        uint8_t code = res == IRR_RT_OK        ? IRR_ACK_OK
                       : res == IRR_RT_CLAMPED ? IRR_ACK_CLAMPED
                                               : IRR_ACK_BAD_ZONE;
        return ack(r, h.counter, code, now_ms, sense_flags, reply, reply_len);
    }
    case IRR_MSG_STOP:
        irr_rt_stop(&r->rt);
        return ack(r, h.counter, IRR_ACK_OK, now_ms, sense_flags, reply, reply_len);
    case IRR_MSG_STATUS_REQ:
        return ack(r, h.counter, IRR_ACK_OK, now_ms, sense_flags, reply, reply_len);
    default:
        return ack(r, h.counter, IRR_ACK_UNSUPPORTED, now_ms, sense_flags, reply, reply_len);
    }
}
