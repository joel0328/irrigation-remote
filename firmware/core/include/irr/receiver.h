/*
 * Receiver message handling: decode, authenticate, replay-check, act on the
 * run timer, and build the reply frame. Pure logic; the port supplies time,
 * radio bytes, sense readings and persistence.
 */
#ifndef IRR_RECEIVER_H
#define IRR_RECEIVER_H

#include <stddef.h>
#include <stdint.h>

#include "irr/aes128.h"
#include "irr/counter.h"
#include "irr/frame.h"
#include "irr/runtimer.h"

/* ACK result codes (payload byte 4). */
#define IRR_ACK_OK          0u
#define IRR_ACK_CLAMPED     1u
#define IRR_ACK_BAD_ZONE    2u
#define IRR_ACK_BAD_PAYLOAD 3u
#define IRR_ACK_UNSUPPORTED 4u

/* Sense flags (ACK payload byte 8). */
#define IRR_SENSE_ZONE_LIVE   0x01u
#define IRR_SENSE_MASTER_LIVE 0x02u
#define IRR_SENSE_NONE        0x80u /* no sense hardware or no reading */

typedef struct {
    irr_aes128_ctx key;
    uint32_t       net_id;
    irr_replay     replay;   /* handheld -> receiver */
    irr_tx_counter tx;       /* receiver -> handheld */
    irr_runtimer   rt;
    uint8_t        last_reply[IRR_MAX_FRAME];
    size_t         last_reply_len;
} irr_receiver;

typedef enum {
    IRR_RX_REPLY = 0,  /* reply frame written, send it */
    IRR_RX_DROP        /* not for us, not authentic, or stale: send nothing */
} irr_rx_action;

void irr_receiver_init(irr_receiver *r, const uint8_t key[IRR_AES_KEY],
                       uint32_t net_id, const irr_rt_config *cfg,
                       uint32_t last_rx_counter, uint32_t next_tx_counter);

/*
 * Handle one received frame. sense_flags is the current voltage-sense
 * reading (IRR_SENSE_*). On IRR_RX_REPLY, reply/reply_len hold the frame to
 * transmit. The caller must persist r->replay.last and r->tx.next.
 */
irr_rx_action irr_receiver_handle(irr_receiver *r, const uint8_t *buf,
                                  size_t len, uint32_t now_ms,
                                  uint8_t sense_flags, uint8_t *reply,
                                  size_t *reply_len);

#endif
