/*
 * utils.c
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */
#include "utils.h"
#include "stm32f4xx_hal.h"

#include <gapcom_core.h>

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
	size_t start_idx = TF_USE_SOF_BYTE + TF_ID_BYTES;
	for (size_t i = 0; i < TF_LEN_BYTES; i++) {
		res <<= 8;
		res |= buf[start_idx + i];
	}
	return res;
}
