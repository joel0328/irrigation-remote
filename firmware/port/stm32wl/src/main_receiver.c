/*
 * Receiver main loop. UNTESTED: runs on stubs only.
 *
 * Order each pass: advance the run timer, apply outputs, then handle radio.
 * Outputs are rewritten every pass so a glitch cannot leave a relay on
 * after the timer says off.
 */
#include <stddef.h>
#include <stdint.h>

#include "board.h"
#include "irr/receiver.h"

static irr_receiver rx;

int main(void)
{
    uint8_t key[16];
    uint32_t net_id;
    irr_rt_config cfg;

    board_init();
    board_set_outputs(0, 0); /* all off before anything else */

    board_load_key(key, &net_id);
    irr_rt_default_config(&cfg);
    irr_receiver_init(&rx, key, net_id, &cfg, board_load_rx_counter(),
                      board_load_tx_counter());

    for (;;) {
        uint32_t now = board_millis();
        uint8_t zone, master;

        irr_rt_tick(&rx.rt, now);
        irr_rt_outputs(&rx.rt, &zone, &master);
        board_set_outputs(zone, master);

        uint8_t buf[IRR_MAX_FRAME], reply[IRR_MAX_FRAME];
        size_t n = board_radio_receive(buf, sizeof buf);
        if (n) {
            size_t rlen;
            if (irr_receiver_handle(&rx, buf, n, now, board_read_sense(), reply, &rlen) ==
                IRR_RX_REPLY) {
                /* Persist before replying so a reset cannot reuse a counter. */
                board_store_counters(rx.replay.last, rx.tx.next);
                irr_rt_outputs(&rx.rt, &zone, &master);
                board_set_outputs(zone, master);
                board_radio_send(reply, rlen);
            }
        }
        board_watchdog_kick();
    }
}
