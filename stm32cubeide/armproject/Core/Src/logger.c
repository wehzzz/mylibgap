/*
 * logger.c
 *
 *  Created on: May 7, 2025
 *      Author: mleve
 */

#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static char TX_Buffer[256];

const char* log_level_to_str(log_level level) {
    switch (level) {
        case LOG_DEBUG:   return "Debug";
        case LOG_INFO:    return "Info";
        case LOG_WARNING: return "Warning";
        case LOG_ERROR:   return "Error";
        default:          return "UNKNOWN";
    }
}

void gap_log(log_level level, const char *fmt, ...) {
	int len = 0;
	len += snprintf(TX_Buffer, sizeof(TX_Buffer), "[%s] ", log_level_to_str(level));

	va_list args;
	va_start(args, fmt);
	len += vsnprintf(TX_Buffer + len, sizeof(TX_Buffer) - len, fmt, args);
	va_end(args);

	snprintf(TX_Buffer + len, sizeof(TX_Buffer) - len, "\r\n");

	HAL_UART_Transmit(&huart1, (uint8_t*) TX_Buffer, strlen(TX_Buffer),
			HAL_MAX_DELAY);
}

