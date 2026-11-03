/* Handheld and receiver talking through an in-memory "radio". */
#include "irr/handheld.h"
#include "irr/receiver.h"
#include "irr/version.h"
#include "minitest.h"

static const uint8_t KEY[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                                0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xf0, 0x01};
#define NET 0xC0FFEE01u

static irr_handheld hh;
static irr_receiver rx;
static uint8_t f[IRR_MAX_FRAME], r[IRR_MAX_FRAME];
static size_t flen, rlen;

static void pair(void)
{
    irr_rt_config cfg;
    irr_rt_default_config(&cfg);
    irr_handheld_init(&hh, KEY, NET, 1, 0);
    irr_receiver_init(&rx, KEY, NET, &cfg, 0, 1);
}

static uint8_t rx_zone(void)
{
    uint8_t z, m;
    irr_rt_outputs(&rx.rt, &z, &m);
    return z;
}

static void test_hello(void)
{
    pair();
    uint32_t c;
    CHECK_EQ(irr_hh_hello(&hh, f, &flen, &c), 0);
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 0, IRR_SENSE_NONE, r, &rlen), IRR_RX_REPLY);
    irr_hello_info info;
    CHECK_EQ(irr_hh_parse_reply(&hh, r, rlen, NULL, &info), 0);
    CHECK_EQ(info.fw_major, IRR_FW_MAJOR);
    CHECK_EQ(info.fw_minor, IRR_FW_MINOR);
    CHECK_EQ(info.fw_flags & 1, IRR_FW_DEV);
    CHECK_EQ(info.zone_count, 8);
}

static void test_start_ack_stop(void)
{
    pair();
    uint32_t c;
    irr_ack a;
    CHECK_EQ(irr_hh_start(&hh, 3, 15, f, &flen, &c), 0);
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 0, IRR_SENSE_ZONE_LIVE, r, &rlen), IRR_RX_REPLY);
    CHECK_EQ(rx_zone(), 3);
    CHECK_EQ(irr_hh_parse_reply(&hh, r, rlen, &a, NULL), 0);
    CHECK_EQ(a.acked_counter, c);
    CHECK_EQ(a.result, IRR_ACK_OK);
    CHECK_EQ(a.zone, 3);
    CHECK_EQ(a.remaining_s, 15 * 60);
    CHECK_EQ(a.sense, IRR_SENSE_ZONE_LIVE);

    CHECK_EQ(irr_hh_stop(&hh, f, &flen, &c), 0);
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 1000, 0, r, &rlen), IRR_RX_REPLY);
    CHECK_EQ(rx_zone(), 0);
    CHECK_EQ(irr_hh_parse_reply(&hh, r, rlen, &a, NULL), 0);
    CHECK_EQ(a.zone, 0);
    CHECK_EQ(a.remaining_s, 0);
}

static void test_replayed_start_is_ignored(void)
{
    pair();
    uint32_t c;
    uint8_t old[IRR_MAX_FRAME];
    size_t oldlen;
    irr_hh_start(&hh, 2, 5, old, &oldlen, &c);
    irr_receiver_handle(&rx, old, oldlen, 0, 0, r, &rlen);
    irr_hh_stop(&hh, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 10, 0, r, &rlen);
    CHECK_EQ(rx_zone(), 0);
    /* An attacker records the start and plays it back later. */
    CHECK_EQ(irr_receiver_handle(&rx, old, oldlen, 20, 0, r, &rlen), IRR_RX_DROP);
    CHECK_EQ(rx_zone(), 0);
}

static void test_retry_gets_same_ack_without_acting_twice(void)
{
    pair();
    uint32_t c;
    irr_hh_start(&hh, 4, 10, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    uint8_t first[IRR_MAX_FRAME];
    size_t firstlen = rlen;
    memcpy(first, r, rlen);
    /* ACK lost; handheld sends the identical frame again. */
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 30000, 0, r, &rlen), IRR_RX_REPLY);
    CHECK_EQ(rlen, firstlen);
    CHECK_MEM(r, first, rlen);
    /* Timer was not restarted by the retry. */
    CHECK_EQ(irr_rt_remaining_s(&rx.rt, 30000), 10 * 60 - 30);
}

static void test_wrong_network_or_key_dropped(void)
{
    pair();
    irr_handheld other;
    uint8_t k2[16] = {0};
    uint32_t c;
    irr_handheld_init(&other, k2, NET, 1, 0);
    irr_hh_start(&other, 1, 5, f, &flen, &c);
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen), IRR_RX_DROP);

    irr_handheld_init(&other, KEY, NET + 1, 1, 0);
    irr_hh_start(&other, 1, 5, f, &flen, &c);
    CHECK_EQ(irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen), IRR_RX_DROP);
    CHECK_EQ(rx_zone(), 0);
}

static void test_reflected_reply_dropped(void)
{
    pair();
    uint32_t c;
    irr_hh_status(&hh, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    /* The receiver's own reply sent back to it must not be accepted. */
    CHECK_EQ(irr_receiver_handle(&rx, r, rlen, 0, 0, f, &flen), IRR_RX_DROP);
}

static void test_bad_zone_and_clamp_reported(void)
{
    pair();
    uint32_t c;
    irr_ack a;
    irr_hh_start(&hh, 9, 5, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    irr_hh_parse_reply(&hh, r, rlen, &a, NULL);
    CHECK_EQ(a.result, IRR_ACK_BAD_ZONE);
    CHECK_EQ(rx_zone(), 0);

    irr_hh_start(&hh, 1, 999, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    irr_hh_parse_reply(&hh, r, rlen, &a, NULL);
    CHECK_EQ(a.result, IRR_ACK_CLAMPED);
    CHECK_EQ(a.remaining_s, 120 * 60);
}

static void test_run_ends_without_any_message(void)
{
    pair();
    uint32_t c;
    irr_hh_start(&hh, 1, 1, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    /* Link lost: nothing more arrives. The port keeps ticking. */
    irr_rt_tick(&rx.rt, 60000);
    CHECK_EQ(rx_zone(), 0);
}

static void test_replayed_ack_rejected_by_handheld(void)
{
    pair();
    uint32_t c;
    irr_ack a;
    irr_hh_status(&hh, f, &flen, &c);
    irr_receiver_handle(&rx, f, flen, 0, 0, r, &rlen);
    CHECK_EQ(irr_hh_parse_reply(&hh, r, rlen, &a, NULL), 0);
    CHECK_EQ(irr_hh_parse_reply(&hh, r, rlen, &a, NULL), -1);
}

int main(void)
{
    RUN(test_hello);
    RUN(test_start_ack_stop);
    RUN(test_replayed_start_is_ignored);
    RUN(test_retry_gets_same_ack_without_acting_twice);
    RUN(test_wrong_network_or_key_dropped);
    RUN(test_reflected_reply_dropped);
    RUN(test_bad_zone_and_clamp_reported);
    RUN(test_run_ends_without_any_message);
    RUN(test_replayed_ack_rejected_by_handheld);
    return MT_DONE();
}
