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

log_level global_level = LOG_INFO;
static char tx_buffer[256];

const char* log_level_to_str(log_level level) {
    switch (level) {
        case LOG_DEBUG:   return "Debug";
        case LOG_INFO:    return "Info";
        case LOG_WARNING: return "Warning";
        case LOG_ERROR:   return "Error";
        default:          return "UNKNOWN";
    }
}

void log(log_levet level, const char* fmt, ...) {
	int len = 0;
	len += snprintf(tx_buffer, sizeof(tx_buffer), "[%s] ", log_level_to_str(level));

	va_list args;
	va_start(args, fmt);
	len += vsnprintf(tx_buffer + len, sizeof(tx_buffer) - len, fmt, args);
	va_end(args);

	snprintf(tx_buffer + len, sizeof(tx_buffer) - len, "\r\n");

	HAL_UART_Transmit_IT(&huart1, (uint8_t *)tx_buffer, strlen(tx_buffer));
}

