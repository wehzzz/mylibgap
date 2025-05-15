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
int get_protobuf_len(const uint8_t *buf);

/**
 * @brief Convert 4 char into an uint32
 * @param data Array of char of length 4
 * @return Computed uint32
 */
uint32_t char_to_uint32(const char* data);

/**
 * @brief Convert an array of uint32 of length WORDS_NUMBER into an array of char
 * @param data Array of uint32 to convert
 * @param result Resulted array of char, must be previously allocated
 */
void uint32_to_char(uint32_t* data, char* result);

/**
 * @brief Verify that the given string is a valid semver (MAJOR.MINOR.PATCH)
 * @param version String to check
 * @return true if it's valid, false otherwise
 */
bool is_valid_semver(const char* version);

#endif /* INC_UTILS_H_ */
