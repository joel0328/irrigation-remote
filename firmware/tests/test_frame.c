#include "irr/counter.h"
#include "irr/frame.h"
#include "irr/version.h"
#include "minitest.h"

static irr_aes128_ctx key;

static void setup(void)
{
    uint8_t k[16];
    for (int i = 0; i < 16; i++) {
        k[i] = (uint8_t)(0xA0 + i); /* test key only */
    }
    irr_aes128_init(&key, k);
}

static void test_round_trip(void)
{
    uint8_t buf[IRR_MAX_FRAME];
    size_t len;
    uint8_t payload[3] = {5, 0x2c, 0x01};
    irr_header h = {0};
    h.type = IRR_MSG_START;
    h.net_id = 0x12345678;
    h.counter = 42;
    CHECK_EQ(irr_frame_encode(&key, &h, payload, 3, buf, &len), IRR_FRAME_OK);
    CHECK_EQ(len, IRR_HEADER_LEN + 3 + IRR_TAG_LEN);
    CHECK_EQ(buf[0], (IRR_PROTO_MAJOR << 4) | IRR_PROTO_MINOR);
    CHECK_EQ(buf[2], 0x78); /* little-endian net_id */
    CHECK_EQ(buf[6], 42);

    irr_header d;
    const uint8_t *p;
    CHECK_EQ(irr_frame_decode(&key, buf, len, &d, &p), IRR_FRAME_OK);
    CHECK_EQ(d.type, IRR_MSG_START);
    CHECK_EQ(d.net_id, 0x12345678);
    CHECK_EQ(d.counter, 42);
    CHECK_EQ(d.len, 3);
    CHECK_MEM(p, payload, 3);
}

static void test_every_bit_flip_rejected(void)
{
    uint8_t buf[IRR_MAX_FRAME];
    size_t len;
    uint8_t payload[3] = {1, 10, 0};
    irr_header h = {0};
    h.type = IRR_MSG_START;
    h.net_id = 7;
    h.counter = 1;
    irr_frame_encode(&key, &h, payload, 3, buf, &len);
    int accepted = 0;
    for (size_t i = 0; i < len; i++) {
        for (int bit = 0; bit < 8; bit++) {
            buf[i] ^= (uint8_t)(1u << bit);
            irr_header d;
            const uint8_t *p;
            if (irr_frame_decode(&key, buf, len, &d, &p) == IRR_FRAME_OK) {
                accepted++;
            }
            buf[i] ^= (uint8_t)(1u << bit);
        }
    }
    CHECK_EQ(accepted, 0);
}

static void test_wrong_key_rejected(void)
{
    uint8_t buf[IRR_MAX_FRAME];
    size_t len;
    irr_header h = {0};
    h.type = IRR_MSG_STOP;
    irr_frame_encode(&key, &h, NULL, 0, buf, &len);
    uint8_t other[16] = {0};
    irr_aes128_ctx k2;
    irr_aes128_init(&k2, other);
    irr_header d;
    const uint8_t *p;
    CHECK_EQ(irr_frame_decode(&k2, buf, len, &d, &p), IRR_FRAME_BAD_TAG);
}

static void test_length_checks(void)
{
    uint8_t buf[IRR_MAX_FRAME + 4];
    size_t len;
    irr_header h = {0};
    h.type = IRR_MSG_STOP;
    irr_frame_encode(&key, &h, NULL, 0, buf, &len);
    irr_header d;
    const uint8_t *p;
    CHECK_EQ(irr_frame_decode(&key, buf, len - 1, &d, &p), IRR_FRAME_SHORT);
    CHECK_EQ(irr_frame_decode(&key, buf, len + 1, &d, &p), IRR_FRAME_BAD_LEN);
    uint8_t big[IRR_MAX_PAYLOAD + 1] = {0};
    CHECK_EQ(irr_frame_encode(&key, &h, big, IRR_MAX_PAYLOAD + 1, buf, &len), IRR_FRAME_TOO_BIG);
    CHECK_EQ(irr_frame_encode(&key, &h, big, IRR_MAX_PAYLOAD, buf, &len), IRR_FRAME_OK);
    CHECK_EQ(len, IRR_MAX_FRAME);
}

static void test_version_mismatch(void)
{
    uint8_t buf[IRR_MAX_FRAME];
    size_t len;
    irr_header h = {0};
    h.type = IRR_MSG_STOP;
    irr_frame_encode(&key, &h, NULL, 0, buf, &len);
    /* Re-sign with a different version byte so only the version differs. */
    buf[0] = (uint8_t)(buf[0] + 1);
    uint8_t tag[16];
    irr_cmac(&key, buf, IRR_HEADER_LEN, tag);
    memcpy(&buf[IRR_HEADER_LEN], tag, IRR_TAG_LEN);
    irr_header d;
    const uint8_t *p;
    CHECK_EQ(irr_frame_decode(&key, buf, len, &d, &p), IRR_FRAME_BAD_VERSION);
}

static void test_replay_window(void)
{
    uint8_t t1[8] = {1}, t2[8] = {2};
    irr_replay r;
    irr_replay_init(&r, 0);
    CHECK_EQ(irr_replay_check(&r, 0, t1), IRR_REPLAY_STALE);
    CHECK_EQ(irr_replay_check(&r, 1, t1), IRR_REPLAY_FRESH);
    irr_replay_accept(&r, 5, t1);
    CHECK_EQ(irr_replay_check(&r, 5, t1), IRR_REPLAY_DUPLICATE);
    CHECK_EQ(irr_replay_check(&r, 5, t2), IRR_REPLAY_STALE);
    CHECK_EQ(irr_replay_check(&r, 4, t1), IRR_REPLAY_STALE);
    CHECK_EQ(irr_replay_check(&r, 6, t2), IRR_REPLAY_FRESH);

    /* After a reset only the counter is restored: no duplicate path. */
    irr_replay_init(&r, 5);
    CHECK_EQ(irr_replay_check(&r, 5, t1), IRR_REPLAY_STALE);
    CHECK_EQ(irr_replay_check(&r, 6, t1), IRR_REPLAY_FRESH);
}

static void test_tx_counter(void)
{
    irr_tx_counter c;
    uint32_t v;
    irr_tx_counter_init(&c, 0);
    CHECK_EQ(irr_tx_counter_take(&c, &v), 0);
    CHECK_EQ(v, 1);
    irr_tx_counter_init(&c, 0xffffffffu);
    CHECK_EQ(irr_tx_counter_take(&c, &v), 0);
    CHECK_EQ(v, 0xffffffffu);
    CHECK_EQ(irr_tx_counter_take(&c, &v), -1);
}

int main(void)
{
    setup();
    RUN(test_round_trip);
    RUN(test_every_bit_flip_rejected);
    RUN(test_wrong_key_rejected);
    RUN(test_length_checks);
    RUN(test_version_mismatch);
    RUN(test_replay_window);
    RUN(test_tx_counter);
    return MT_DONE();
}
