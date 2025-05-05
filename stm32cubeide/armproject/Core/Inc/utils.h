/*
 * utils.h
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stddef.h>
#include <stdint.h>

/**
 * Put LD3 (PG13) on for 200 ms
 */
void flash_led_tx_complete();

/**
 * Put LD3 (PG13) on for 50 ms
 */
void flash_led_rx_complete();

/**
 * Put LD4 (PG14) on for 100 ms
 */
void flash_red_led();

/**
 * Compute the protocol buffer length to receive
 */
size_t get_protobuf_len(const uint8_t *buf);

#endif /* INC_UTILS_H_ */
