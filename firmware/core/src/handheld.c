#include "irr/handheld.h"

#include "irr/version.h"

void irr_handheld_init(irr_handheld *h, const uint8_t key[IRR_AES_KEY], uint32_t net_id,
                       uint32_t next_tx_counter, uint32_t last_rx_counter)
{
    irr_aes128_init(&h->key, key);
    h->net_id = net_id;
    irr_tx_counter_init(&h->tx, next_tx_counter);
    irr_replay_init(&h->replay, last_rx_counter);
}

static int build(irr_handheld *h, uint8_t type, const uint8_t *p, uint8_t plen,
                 uint8_t *out, size_t *out_len, uint32_t *counter_out)
{
    irr_header hdr = {0};
    hdr.type = type;
    hdr.net_id = h->net_id;
    if (irr_tx_counter_take(&h->tx, &hdr.counter) != 0) {
        return -1;
    }
    if (irr_frame_encode(&h->key, &hdr, p, plen, out, out_len) != IRR_FRAME_OK) {
        return -1;
    }
    if (counter_out) {
        *counter_out = hdr.counter;
    }
    return 0;
}

int irr_hh_hello(irr_handheld *h, uint8_t *out, size_t *out_len, uint32_t *counter_out)
{
    uint8_t p[4] = {IRR_FW_MAJOR, IRR_FW_MINOR, IRR_FW_PATCH, IRR_FW_DEV ? 1u : 0u};
    return build(h, IRR_MSG_HELLO, p, sizeof p, out, out_len, counter_out);
}

int irr_hh_start(irr_handheld *h, uint8_t zone, uint16_t minutes, uint8_t *out,
                 size_t *out_len, uint32_t *counter_out)
{
    if (zone == 0) {
        return -1;
    }
    uint8_t p[3];
    p[0] = zone;
    irr_put_u16(&p[1], minutes);
    return build(h, IRR_MSG_START, p, sizeof p, out, out_len, counter_out);
}

int irr_hh_stop(irr_handheld *h, uint8_t *out, size_t *out_len, uint32_t *counter_out)
{
    return build(h, IRR_MSG_STOP, NULL, 0, out, out_len, counter_out);
}

int irr_hh_status(irr_handheld *h, uint8_t *out, size_t *out_len, uint32_t *counter_out)
{
    return build(h, IRR_MSG_STATUS_REQ, NULL, 0, out, out_len, counter_out);
}

int irr_hh_parse_reply(irr_handheld *h, const uint8_t *buf, size_t len, irr_ack *ack,
                       irr_hello_info *hello)
{
    irr_header hdr;
    const uint8_t *pl;
    if (irr_frame_decode(&h->key, buf, len, &hdr, &pl) != IRR_FRAME_OK) {
        return -1;
    }
    if (hdr.net_id != h->net_id || !(hdr.type & IRR_DIR_FROM_RX)) {
        return -1;
    }
    const uint8_t *tag = &buf[len - IRR_TAG_LEN];
    if (irr_replay_check(&h->replay, hdr.counter, tag) != IRR_REPLAY_FRESH) {
        return -1;
    }
    if (hdr.type == IRR_MSG_ACK && hdr.len == 9 && ack) {
        ack->acked_counter = irr_get_u32(&pl[0]);
        ack->result = pl[4];
        ack->zone = pl[5];
        ack->remaining_s = irr_get_u16(&pl[6]);
        ack->sense = pl[8];
    } else if (hdr.type == IRR_MSG_HELLO_REPLY && hdr.len == 5 && hello) {
        hello->fw_major = pl[0];
        hello->fw_minor = pl[1];
        hello->fw_patch = pl[2];
        hello->fw_flags = pl[3];
        hello->zone_count = pl[4];
    } else {
        return -1;
    }
    irr_replay_accept(&h->replay, hdr.counter, tag);
    return 0;
}
