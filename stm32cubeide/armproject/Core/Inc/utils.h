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
#include <stdbool.h>

/**
 * @brief Put LD3 (PG13) on for 200 ms
 */
void flash_led_tx_complete();

/**
 * @brief Put LD3 (PG13) on for 50 ms
 */
void flash_led_rx_complete();

/**
 * @brief Put LD4 (PG14) on for 100 ms
 */
void flash_red_led();

/**
 * @brief Compute the protocol buffer length to receive
 * @param buf Current RX_Buffer filled with header part
 * @return length of the protocol buffer to receive
 */
size_t get_protobuf_len(const uint8_t *buf);

uint32_t char_to_uint32(const char* data);

void uint32_to_char(uint32_t* data, char* result);

bool is_valid_semver(const char* version);

#endif /* INC_UTILS_H_ */
