/*
 * Rolling-counter replay protection.
 *
 * Each direction has its own counter. A receiver accepts an authenticated
 * frame only if its counter is greater than the last one accepted. A frame
 * whose counter and tag both equal the last accepted frame is a retry of the
 * same command: it is reported as a duplicate so the caller can re-send the
 * acknowledgement without acting twice.
 *
 * Persistence is the caller's job. The sender must store its counter (or a
 * reserved block of counters) before transmitting so a reset never reuses a
 * value. See docs/protocol/v0.1.md, "Counters".
 */
#ifndef IRR_COUNTER_H
#define IRR_COUNTER_H

#include <stdint.h>

#include "irr/frame.h"

typedef struct {
    uint32_t last;
    uint8_t  last_tag[IRR_TAG_LEN];
    uint8_t  have_last;
} irr_replay;

typedef enum {
    IRR_REPLAY_FRESH = 0, /* new: accept and act */
    IRR_REPLAY_DUPLICATE, /* same frame again: re-ack, do not act */
    IRR_REPLAY_STALE      /* old or reused counter: drop */
} irr_replay_result;

/* last_accepted is restored from storage; pass 0 for a new pairing. */
void irr_replay_init(irr_replay *r, uint32_t last_accepted);

/* Classify a frame that has already passed irr_frame_decode. */
irr_replay_result irr_replay_check(const irr_replay *r, uint32_t counter,
                                   const uint8_t tag[IRR_TAG_LEN]);

/* Record a fresh frame as accepted. */
void irr_replay_accept(irr_replay *r, uint32_t counter,
                       const uint8_t tag[IRR_TAG_LEN]);

/*
 * Sender side. Counter values start at 1; 0 is never sent.
 * Returns 0 and sets *out, or returns -1 when the counter is exhausted
 * (the pair must be re-keyed).
 */
typedef struct {
    uint32_t next;
} irr_tx_counter;

void irr_tx_counter_init(irr_tx_counter *c, uint32_t next);
int  irr_tx_counter_take(irr_tx_counter *c, uint32_t *out);

#endif
