/*
 * Handheld side: build signed commands and parse replies.
 */
#ifndef IRR_HANDHELD_H
#define IRR_HANDHELD_H

#include <stddef.h>
#include <stdint.h>

#include "irr/aes128.h"
#include "irr/counter.h"
#include "irr/frame.h"

typedef struct {
    irr_aes128_ctx key;
    uint32_t       net_id;
    irr_tx_counter tx;     /* handheld -> receiver */
    irr_replay     replay; /* receiver -> handheld */
} irr_handheld;

typedef struct {
    uint32_t acked_counter;
    uint8_t  result;
    uint8_t  zone;
    uint16_t remaining_s;
    uint8_t  sense;
} irr_ack;

typedef struct {
    uint8_t fw_major, fw_minor, fw_patch, fw_flags;
    uint8_t zone_count;
} irr_hello_info;

void irr_handheld_init(irr_handheld *h, const uint8_t key[IRR_AES_KEY],
                       uint32_t net_id, uint32_t next_tx_counter,
                       uint32_t last_rx_counter);

/* Each returns 0 on success, -1 on counter exhaustion or bad arguments.
 * *counter_out is the counter used, to match against the ACK. */
int irr_hh_hello(irr_handheld *h, uint8_t *out, size_t *out_len,
                 uint32_t *counter_out);
int irr_hh_start(irr_handheld *h, uint8_t zone, uint16_t minutes,
                 uint8_t *out, size_t *out_len, uint32_t *counter_out);
int irr_hh_stop(irr_handheld *h, uint8_t *out, size_t *out_len,
                uint32_t *counter_out);
int irr_hh_status(irr_handheld *h, uint8_t *out, size_t *out_len,
                  uint32_t *counter_out);

/* Parse a reply. Returns 0 if it is an authentic, fresh ACK (or HELLO_REPLY,
 * written to *hello if non-NULL); -1 otherwise. */
int irr_hh_parse_reply(irr_handheld *h, const uint8_t *buf, size_t len,
                       irr_ack *ack, irr_hello_info *hello);

#endif
