/*
 * logger.h
 *
 *  Created on: May 7, 2025
 *      Author: mleve
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <gapcom_core.h>

#include "stm32f4xx_hal.h"

#define LOG_MAX 4

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
} log_level;

extern log_level global_level;
extern UART_HandleTypeDef huart1;

void gap_log(log_level level, const char *fmt, ...);

#endif /* INC_LOGGER_H_ */
