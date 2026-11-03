#include "irr/runtimer.h"
#include "minitest.h"

#define MIN 60000u

static irr_runtimer rt;

static void fresh(void)
{
    irr_rt_config cfg;
    irr_rt_default_config(&cfg);
    irr_rt_init(&rt, &cfg);
}

static uint8_t zone_on(void)
{
    uint8_t z, m;
    irr_rt_outputs(&rt, &z, &m);
    return z;
}

static uint8_t master_on(void)
{
    uint8_t z, m;
    irr_rt_outputs(&rt, &z, &m);
    return m;
}

static void test_power_up_all_off(void)
{
    fresh();
    CHECK_EQ(zone_on(), 0);
    CHECK_EQ(master_on(), 0);
    CHECK_EQ(irr_rt_remaining_s(&rt, 0), 0);
}

static void test_run_ends_by_itself(void)
{
    fresh();
    CHECK_EQ(irr_rt_start(&rt, 3, 10, 1000), IRR_RT_OK);
    CHECK_EQ(zone_on(), 3);
    CHECK_EQ(master_on(), 1);
    CHECK_EQ(irr_rt_remaining_s(&rt, 1000), 600);
    irr_rt_tick(&rt, 1000 + 10 * MIN - 1);
    CHECK_EQ(zone_on(), 3);
    CHECK_EQ(irr_rt_remaining_s(&rt, 1000 + 10 * MIN - 1), 1);
    irr_rt_tick(&rt, 1000 + 10 * MIN);
    CHECK_EQ(zone_on(), 0);
    CHECK_EQ(master_on(), 0);
}

static void test_default_and_clamp(void)
{
    fresh();
    CHECK_EQ(irr_rt_start(&rt, 1, 0, 0), IRR_RT_OK);
    CHECK_EQ(irr_rt_remaining_s(&rt, 0), 20 * 60);
    CHECK_EQ(irr_rt_start(&rt, 1, 500, 0), IRR_RT_CLAMPED);
    CHECK_EQ(irr_rt_remaining_s(&rt, 0), 120 * 60);
}

static void test_bad_zone_changes_nothing(void)
{
    fresh();
    irr_rt_start(&rt, 2, 5, 0);
    CHECK_EQ(irr_rt_start(&rt, 0, 5, 10), IRR_RT_BAD_ZONE);
    CHECK_EQ(irr_rt_start(&rt, 9, 5, 10), IRR_RT_BAD_ZONE);
    CHECK_EQ(zone_on(), 2);
}

static void test_switch_is_break_before_make(void)
{
    fresh();
    irr_rt_start(&rt, 1, 10, 0);
    irr_rt_start(&rt, 2, 10, 5000);
    CHECK_EQ(rt.state, IRR_RT_SWITCHING);
    CHECK_EQ(zone_on(), 0);   /* old zone off at once */
    CHECK_EQ(master_on(), 0);
    irr_rt_tick(&rt, 5199);
    CHECK_EQ(zone_on(), 0);
    irr_rt_tick(&rt, 5200);
    CHECK_EQ(zone_on(), 2);
    /* The new run's full length starts after the dead time. */
    CHECK_EQ(irr_rt_remaining_s(&rt, 5200), 600);
}

static void test_never_two_zones(void)
{
    fresh();
    for (uint32_t t = 0, z = 1; t < 20000; t += 50, z = (z % 8) + 1) {
        if (t % 300 == 0) {
            irr_rt_start(&rt, (uint8_t)z, 1, t);
        }
        irr_rt_tick(&rt, t);
        uint8_t on = zone_on();
        CHECK(on == 0 || on == rt.zone);
    }
}

static void test_same_zone_restarts_timer(void)
{
    fresh();
    irr_rt_start(&rt, 4, 1, 0);
    irr_rt_tick(&rt, 50000);
    irr_rt_start(&rt, 4, 1, 50000);
    CHECK_EQ(rt.state, IRR_RT_RUNNING); /* no off-gap */
    irr_rt_tick(&rt, 50000 + MIN - 1);
    CHECK_EQ(zone_on(), 4);
    irr_rt_tick(&rt, 50000 + MIN);
    CHECK_EQ(zone_on(), 0);
}

static void test_stop(void)
{
    fresh();
    irr_rt_start(&rt, 5, 30, 0);
    irr_rt_stop(&rt);
    CHECK_EQ(zone_on(), 0);
    irr_rt_start(&rt, 5, 30, 0);
    irr_rt_start(&rt, 6, 30, 10);
    irr_rt_stop(&rt); /* stop during the dead time */
    irr_rt_tick(&rt, 10000);
    CHECK_EQ(zone_on(), 0);
}

static void test_clock_wrap(void)
{
    fresh();
    uint32_t start = 0xffffffffu - 30000u;
    irr_rt_start(&rt, 1, 1, start);
    irr_rt_tick(&rt, start + 59999u); /* wraps past zero */
    CHECK_EQ(zone_on(), 1);
    irr_rt_tick(&rt, start + MIN);
    CHECK_EQ(zone_on(), 0);
}

static void test_master_disabled(void)
{
    irr_rt_config cfg;
    irr_rt_default_config(&cfg);
    cfg.master_enabled = 0;
    irr_rt_init(&rt, &cfg);
    irr_rt_start(&rt, 1, 1, 0);
    CHECK_EQ(zone_on(), 1);
    CHECK_EQ(master_on(), 0);
}

int main(void)
{
    RUN(test_power_up_all_off);
    RUN(test_run_ends_by_itself);
    RUN(test_default_and_clamp);
    RUN(test_bad_zone_changes_nothing);
    RUN(test_switch_is_break_before_make);
    RUN(test_never_two_zones);
    RUN(test_same_zone_restarts_timer);
    RUN(test_stop);
    RUN(test_clock_wrap);
    RUN(test_master_disabled);
    return MT_DONE();
}
