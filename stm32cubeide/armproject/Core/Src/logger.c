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
#include <stdlib.h>

static char TX_Buffer[256];

/**
 * @brief Convert the enum log_level into string
 * @param level log level
 * @return the string according to the given level
 */
static const char* log_level_to_str(log_level level) {
	switch (level) {
	case LOG_DEBUG:
		return "Debug";
	case LOG_INFO:
		return "Info";
	case LOG_WARNING:
		return "Warning";
	case LOG_ERROR:
		return "Error";
	default:
		return "UNKNOWN";
	}
}

void gap_log(log_level level, const char *fmt, ...) {
	// Log according to the current verbosity
	if (level < global_level)
		return;

	int len = 0;
	len += snprintf(TX_Buffer, sizeof(TX_Buffer), "[%s] ",
			log_level_to_str(level));

	va_list args;
	va_start(args, fmt);
	len += vsnprintf(TX_Buffer + len, sizeof(TX_Buffer) - (size_t) len, fmt,
			args);
	va_end(args);

	snprintf(TX_Buffer + len, sizeof(TX_Buffer) - (size_t) len, "\r\n");

	HAL_UART_Transmit(&huart1, (uint8_t*) TX_Buffer,
			(uint16_t) strlen(TX_Buffer),
			HAL_MAX_DELAY);
}

void gap_log_gyro(GyroData *data) {
	char sx[8], sy[8], sz[8];

	if (data->x < 0)
		snprintf(sx, sizeof(sx), "-X%05d", abs(data->x));
	else
		snprintf(sx, sizeof(sx), "X%05d", data->x);

	if (data->y < 0)
		snprintf(sy, sizeof(sy), "-Y%05d", abs(data->y));
	else
		snprintf(sy, sizeof(sy), "Y%05d", data->y);

	if (data->z < 0)
		snprintf(sz, sizeof(sz), "-Z%05d", abs(data->z));
	else
		snprintf(sz, sizeof(sz), "Z%05d", data->z);

	gap_log(LOG_INFO, "%s %s %s", sx, sy, sz);
}

