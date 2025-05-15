/*
 * communicator.c
 *
 *  Created on: Apr 24, 2025
 *      Author: lb
 */

#include <communicator.h>
#include <communicator_impl_server.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart7;

int open(communicator_t *self, const char *path) {
	// Nothing to do on server side ?
	UNUSED(self);
	UNUSED(path);
	return 0;
}

int close(communicator_t *self) {
	// Nothing to do on server side ?
	UNUSED(self);
	return 0;
}

ssize_t send(communicator_t *self, const uint8_t *buf, size_t len) {
	UNUSED(self);
	return HAL_UART_Transmit_IT(&huart7, buf, (uint16_t) len);
}

ssize_t recv(communicator_t *self, uint8_t *buf, size_t len) {
	UNUSED(self);
	return HAL_UART_Receive_IT(&huart7, buf, (uint16_t) len);
}

communicator_t* communicator_impl_server_get() {
	static communicator_t com = { open, close, send, recv };
	return &com;
}

