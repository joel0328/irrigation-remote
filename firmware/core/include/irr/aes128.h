/*
 * Software AES-128 (encrypt direction only) and AES-128-CMAC (RFC 4493).
 *
 * This is a portable reference so the protocol can be tested on a PC.
 * It is not hardened against timing or power side channels. On the STM32WL
 * the port is expected to use the hardware AES peripheral instead, checked
 * against the same test vectors.
 */
#ifndef IRR_AES128_H
#define IRR_AES128_H

#include <stddef.h>
#include <stdint.h>

#define IRR_AES_BLOCK 16
#define IRR_AES_KEY   16

typedef struct {
    uint8_t round_keys[176];
} irr_aes128_ctx;

void irr_aes128_init(irr_aes128_ctx *ctx, const uint8_t key[IRR_AES_KEY]);
void irr_aes128_encrypt(const irr_aes128_ctx *ctx,
                        const uint8_t in[IRR_AES_BLOCK],
                        uint8_t out[IRR_AES_BLOCK]);

/* Full 16-byte CMAC tag over msg[0..len). */
void irr_cmac(const irr_aes128_ctx *ctx, const uint8_t *msg, size_t len,
              uint8_t tag[IRR_AES_BLOCK]);

/* Compare without an early exit. Returns 1 if equal. */
int irr_ct_equal(const uint8_t *a, const uint8_t *b, size_t len);

#endif
