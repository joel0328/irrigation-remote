/*
 * STUBS. None of these touch hardware yet. They exist so the port links and
 * CI can check that the core builds for Cortex-M4. Replace each with a real
 * driver (STM32CubeWL HAL/LL or registers) and test it on a board.
 */
#include "board.h"

#include <string.h>

#include "irr/receiver.h"

void board_init(void) {}

uint32_t board_millis(void)
{
    return 0; /* TODO: SysTick at 1 kHz */
}

void board_set_outputs(uint8_t zone, uint8_t master)
{
    (void)zone; /* TODO: relay GPIOs */
    (void)master;
}

uint8_t board_read_sense(void)
{
    return IRR_SENSE_NONE; /* TODO: opto or ADC voltage sense */
}

size_t board_radio_receive(uint8_t *buf, size_t max)
{
    (void)buf; /* TODO: SubGHz radio in LoRa mode */
    (void)max;
    return 0;
}

void board_radio_send(const uint8_t *buf, size_t len)
{
    (void)buf;
    (void)len;
}

uint32_t board_load_rx_counter(void) { return 0; }
uint32_t board_load_tx_counter(void) { return 1; }

void board_store_counters(uint32_t last_rx, uint32_t next_tx)
{
    (void)last_rx; /* TODO: flash with wear levelling */
    (void)next_tx;
}

void board_load_key(uint8_t key[16], uint32_t *net_id)
{
    /* PLACEHOLDER KEY. Not a secret and must never be used in the field. */
    memset(key, 0, 16);
    *net_id = 0;
}

void board_watchdog_kick(void) {}
