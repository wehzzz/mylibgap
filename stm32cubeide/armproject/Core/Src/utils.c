/*
 * utils.c
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */
#include "utils.h"

#include <gapcom_core.h>
#include "stm32f4xx_hal.h"
#include "version.h"

static void flash_gpio(GPIO_TypeDef *gpio, uint16_t pin, uint32_t delay) {
	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
	HAL_Delay(delay);
	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

void flash_led_tx_complete() {
	flash_gpio(GPIOG, GPIO_PIN_13, 200);
}

void flash_led_rx_complete() {
	flash_gpio(GPIOG, GPIO_PIN_13, 50);
}

void flash_red_led() {
	flash_gpio(GPIOG, GPIO_PIN_14, 1000);
}

size_t get_protobuf_len(const uint8_t *buf) {
	size_t res = 0;
	int start_idx = TF_USE_SOF_BYTE + TF_ID_BYTES;
	for (int i = 0; i < TF_LEN_BYTES; i++) {
		res <<= 8;
		res |= buf[start_idx + i];
	}
	return res;
}

uint32_t char_to_uint32(const char* data) {
    return ((uint32_t)(data)[0] << 24) |
           ((uint32_t)(data)[1] << 16) |
           ((uint32_t)(data)[2] << 8)  |
           ((uint32_t)(data)[3]);
}

void uint32_to_char(uint32_t* data, char* result) {
	for (uint32_t word = 0; word < WORDS_NUMBER; word++) {
		for (uint32_t i = 0; i < 4; i++) {
			result[word * 4 + i] = (char)((data[word] >> (8 * (3 - i))) & 0xff);
		}
	}
	result[(WORDS_NUMBER * 4) - 1] = '\0';
}

static bool is_numeric_identifier(const char* version, size_t start, size_t end) {
	if (start >= end) return false;
	if (version[start] == '0' && (end - start) == 1) {
		return true;
	}
	for (size_t i = start; i < end; i++) {
		if (i == start && version[i] == '0')
			return false;
		if ('0' > version[i] || version[i] > '9')
			return false;
	}
	return true;
}

bool is_valid_semver(const char* version) {
	int dot_count = 0;
	size_t start = 0;
	size_t end = 0;
	size_t len = strlen(version);

	for (; end < len; end++) {
		if (version[end] == '.') {
			dot_count++;
			if (!is_numeric_identifier(version, start, end)) {
				return false;
			}
			start = end + 1;
		}
	}
	return dot_count == 2 && is_numeric_identifier(version, start, end);
}
