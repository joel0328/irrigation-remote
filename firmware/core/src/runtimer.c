#include "irr/runtimer.h"

#define MS_PER_MIN 60000u

/* True when time a is at or after time b, allowing for wrap-around. */
static int at_or_after(uint32_t a, uint32_t b)
{
    return (int32_t)(a - b) >= 0;
}

void irr_rt_default_config(irr_rt_config *cfg)
{
    cfg->zone_count = 8;
    cfg->default_min = 20;
    cfg->max_min = 120;
    cfg->dead_time_ms = 200;
    cfg->master_enabled = 1;
}

void irr_rt_init(irr_runtimer *rt, const irr_rt_config *cfg)
{
    rt->cfg = *cfg;
    rt->state = IRR_RT_IDLE;
    rt->zone = 0;
    rt->run_ms = 0;
    rt->t_mark = 0;
}

irr_rt_result irr_rt_start(irr_runtimer *rt, uint8_t zone, uint16_t minutes, uint32_t now_ms)
{
    if (zone == 0 || zone > rt->cfg.zone_count) {
        return IRR_RT_BAD_ZONE;
    }
    irr_rt_result res = IRR_RT_OK;
    if (minutes == 0) {
        minutes = rt->cfg.default_min;
    }
    if (minutes > rt->cfg.max_min) {
        minutes = rt->cfg.max_min;
        res = IRR_RT_CLAMPED;
    }
    rt->run_ms = (uint32_t)minutes * MS_PER_MIN;

    if (rt->state == IRR_RT_RUNNING && rt->zone == zone) {
        /* Same zone: restart its timer without switching it off. */
        rt->t_mark = now_ms;
    } else if (rt->state == IRR_RT_IDLE || rt->cfg.dead_time_ms == 0) {
        rt->state = IRR_RT_RUNNING;
        rt->zone = zone;
        rt->t_mark = now_ms;
    } else if (rt->state == IRR_RT_RUNNING) {
        /* Different zone: everything off, wait out the dead time. */
        rt->state = IRR_RT_SWITCHING;
        rt->zone = zone;
        rt->t_mark = now_ms;
    } else {
        /* Already switching: retarget, keep the dead time already served. */
        rt->zone = zone;
    }
    return res;
}

void irr_rt_stop(irr_runtimer *rt)
{
    rt->state = IRR_RT_IDLE;
    rt->zone = 0;
    rt->run_ms = 0;
}

void irr_rt_tick(irr_runtimer *rt, uint32_t now_ms)
{
    if (rt->state == IRR_RT_SWITCHING &&
        at_or_after(now_ms, rt->t_mark + rt->cfg.dead_time_ms)) {
        rt->state = IRR_RT_RUNNING;
        rt->t_mark = now_ms;
    }
    if (rt->state == IRR_RT_RUNNING && at_or_after(now_ms, rt->t_mark + rt->run_ms)) {
        irr_rt_stop(rt);
    }
}

void irr_rt_outputs(const irr_runtimer *rt, uint8_t *zone, uint8_t *master)
{
    int on = rt->state == IRR_RT_RUNNING;
    *zone = on ? rt->zone : 0;
    *master = (uint8_t)(on && rt->cfg.master_enabled);
}

uint32_t irr_rt_remaining_s(const irr_runtimer *rt, uint32_t now_ms)
{
    switch (rt->state) {
    case IRR_RT_SWITCHING:
        return (rt->run_ms + 999u) / 1000u;
    case IRR_RT_RUNNING: {
        uint32_t end = rt->t_mark + rt->run_ms;
        if (at_or_after(now_ms, end)) {
            return 0;
        }
        return (end - now_ms + 999u) / 1000u;
    }
    default:
        return 0;
    }
}
