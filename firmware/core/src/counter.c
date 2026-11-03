#include "irr/counter.h"

#include <string.h>

void irr_replay_init(irr_replay *r, uint32_t last_accepted)
{
    r->last = last_accepted;
    memset(r->last_tag, 0, sizeof r->last_tag);
    r->have_last = 0; /* the tag is not persisted, so no duplicates after reset */
}

irr_replay_result irr_replay_check(const irr_replay *r, uint32_t counter,
                                   const uint8_t tag[IRR_TAG_LEN])
{
    if (counter == 0) {
        return IRR_REPLAY_STALE;
    }
    if (counter > r->last) {
        return IRR_REPLAY_FRESH;
    }
    if (counter == r->last && r->have_last && irr_ct_equal(tag, r->last_tag, IRR_TAG_LEN)) {
        return IRR_REPLAY_DUPLICATE;
    }
    return IRR_REPLAY_STALE;
}

void irr_replay_accept(irr_replay *r, uint32_t counter, const uint8_t tag[IRR_TAG_LEN])
{
    r->last = counter;
    memcpy(r->last_tag, tag, IRR_TAG_LEN);
    r->have_last = 1;
}

void irr_tx_counter_init(irr_tx_counter *c, uint32_t next)
{
    c->next = next == 0 ? 1 : next;
}

int irr_tx_counter_take(irr_tx_counter *c, uint32_t *out)
{
    if (c->next == 0) { /* wrapped past UINT32_MAX */
        return -1;
    }
    *out = c->next++;
    return 0;
}
