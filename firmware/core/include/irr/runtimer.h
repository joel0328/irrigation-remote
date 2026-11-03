/*
 * Receiver run-timer state machine. No hardware calls: the caller feeds it
 * time and commands, then reads back which outputs should be on.
 *
 * Rules (REQ-08, REQ-09, REQ-10):
 *  - Only one zone is on at a time.
 *  - Every run has an end time held here. When it passes, outputs go off.
 *    This is the fail-safe for a lost link: no further message is needed
 *    to stop a run.
 *  - Switching zones is break-before-make: all outputs off for dead_time_ms
 *    before the new zone turns on.
 *  - Run time 0 means the default; anything over the maximum is clamped.
 *  - After init (power-up) everything is off.
 *
 * Time is a free-running uint32_t millisecond counter; wrap-around is handled.
 */
#ifndef IRR_RUNTIMER_H
#define IRR_RUNTIMER_H

#include <stdint.h>

typedef struct {
    uint8_t  zone_count;     /* zones are numbered 1..zone_count */
    uint16_t default_min;    /* used when a start asks for 0 minutes */
    uint16_t max_min;        /* longer requests are clamped */
    uint16_t dead_time_ms;   /* break-before-make gap when switching */
    uint8_t  master_enabled; /* master valve / pump follows the zone */
} irr_rt_config;

typedef enum {
    IRR_RT_IDLE = 0,
    IRR_RT_SWITCHING, /* outputs off, waiting out the dead time */
    IRR_RT_RUNNING
} irr_rt_state;

typedef enum {
    IRR_RT_OK = 0,
    IRR_RT_CLAMPED,  /* started, but run time was cut to max_min */
    IRR_RT_BAD_ZONE  /* zone out of range, nothing changed */
} irr_rt_result;

typedef struct {
    irr_rt_config cfg;
    irr_rt_state  state;
    uint8_t       zone;        /* active or pending zone, 0 when idle */
    uint32_t      run_ms;      /* duration of the current run */
    uint32_t      t_mark;      /* switching: dead time start; running: run start */
} irr_runtimer;

/* Fills defaults: 8 zones, 20 min default, 120 min max, 200 ms dead time,
 * master enabled. */
void irr_rt_default_config(irr_rt_config *cfg);

void irr_rt_init(irr_runtimer *rt, const irr_rt_config *cfg);

irr_rt_result irr_rt_start(irr_runtimer *rt, uint8_t zone, uint16_t minutes,
                           uint32_t now_ms);
void irr_rt_stop(irr_runtimer *rt);

/* Advance time. Call often (every few ms to every 100 ms is fine). */
void irr_rt_tick(irr_runtimer *rt, uint32_t now_ms);

/* Desired outputs. *zone is 0 when no zone should be on. */
void irr_rt_outputs(const irr_runtimer *rt, uint8_t *zone, uint8_t *master);

/* Seconds left in the current run, rounded up; 0 when idle. Includes the
 * pending run while switching. */
uint32_t irr_rt_remaining_s(const irr_runtimer *rt, uint32_t now_ms);

#endif
