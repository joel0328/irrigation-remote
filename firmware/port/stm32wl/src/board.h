/*
 * Board layer for the receiver. The portable core never calls these; only
 * main_receiver.c does. Every function is a stub today (board_stub.c).
 */
#ifndef BOARD_H
#define BOARD_H

#include <stddef.h>
#include <stdint.h>

void board_init(void);

/* Free-running millisecond counter (SysTick or LPTIM). */
uint32_t board_millis(void);

/* Drive outputs. zone 0 means all zone relays off. Must be safe to call
 * every loop. Relays must be normally open so loss of power is all-off. */
void board_set_outputs(uint8_t zone, uint8_t master);

/* Voltage-sense reading as IRR_SENSE_* flags. */
uint8_t board_read_sense(void);

/* Radio. receive returns the frame length, or 0 if nothing arrived. */
size_t board_radio_receive(uint8_t *buf, size_t max);
void board_radio_send(const uint8_t *buf, size_t len);

/* Persistent storage for counters (flash or EEPROM emulation). */
uint32_t board_load_rx_counter(void);
uint32_t board_load_tx_counter(void);
void board_store_counters(uint32_t last_rx, uint32_t next_tx);

/* Key provisioning. Placeholder until a key-management plan exists. */
void board_load_key(uint8_t key[16], uint32_t *net_id);

void board_watchdog_kick(void);

#endif
