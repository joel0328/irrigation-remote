/*
 * Radio frame encoding and authentication, protocol v0.1.
 * Wire format is specified in docs/protocol/v0.1.md. Keep them in step.
 *
 *   0      ver      (major << 4) | minor
 *   1      type     bit 7 set = sent by the receiver
 *   2..5   net_id   pairing identifier, little-endian
 *   6..9   counter  sender's rolling counter, little-endian
 *   10     len      payload length, 0..IRR_MAX_PAYLOAD
 *   11..   payload
 *   last 8 tag      first 8 bytes of AES-128-CMAC over bytes 0..(10+len)
 */
#ifndef IRR_FRAME_H
#define IRR_FRAME_H

#include <stddef.h>
#include <stdint.h>

#include "irr/aes128.h"

#define IRR_HEADER_LEN  11
#define IRR_TAG_LEN     8
#define IRR_MAX_PAYLOAD 32
#define IRR_MAX_FRAME   (IRR_HEADER_LEN + IRR_MAX_PAYLOAD + IRR_TAG_LEN)

#define IRR_DIR_FROM_RX 0x80u

/* Message types. Handheld to receiver: */
#define IRR_MSG_HELLO      0x01u
#define IRR_MSG_START      0x02u
#define IRR_MSG_STOP       0x03u
#define IRR_MSG_STATUS_REQ 0x04u
/* Receiver to handheld: */
#define IRR_MSG_HELLO_REPLY (IRR_DIR_FROM_RX | 0x01u)
#define IRR_MSG_ACK         (IRR_DIR_FROM_RX | 0x02u)

typedef struct {
    uint8_t  ver;
    uint8_t  type;
    uint32_t net_id;
    uint32_t counter;
    uint8_t  len;
} irr_header;

typedef enum {
    IRR_FRAME_OK = 0,
    IRR_FRAME_SHORT,       /* buffer shorter than header + tag */
    IRR_FRAME_BAD_LEN,     /* len field disagrees with buffer size */
    IRR_FRAME_BAD_VERSION, /* protocol version not accepted */
    IRR_FRAME_BAD_TAG,     /* authentication failed */
    IRR_FRAME_TOO_BIG      /* payload over IRR_MAX_PAYLOAD (encode) */
} irr_frame_result;

/* Version byte for this build's protocol version. */
uint8_t irr_proto_ver_byte(void);

/*
 * Encode and sign. hdr->ver is overwritten with this build's version and
 * hdr->len with payload_len. out must hold IRR_MAX_FRAME bytes.
 */
irr_frame_result irr_frame_encode(const irr_aes128_ctx *key, irr_header *hdr,
                                  const uint8_t *payload, uint8_t payload_len,
                                  uint8_t *out, size_t *out_len);

/*
 * Check length, version and tag, then fill hdr and point *payload into buf.
 * Nothing is filled unless the result is IRR_FRAME_OK. Counter freshness is
 * not checked here; see counter.h.
 */
irr_frame_result irr_frame_decode(const irr_aes128_ctx *key, const uint8_t *buf,
                                  size_t len, irr_header *hdr,
                                  const uint8_t **payload);

/* Little-endian helpers, also used for payloads. */
void     irr_put_u16(uint8_t *p, uint16_t v);
void     irr_put_u32(uint8_t *p, uint32_t v);
uint16_t irr_get_u16(const uint8_t *p);
uint32_t irr_get_u32(const uint8_t *p);

#endif
