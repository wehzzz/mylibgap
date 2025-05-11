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
#include "mpu6050.h"

#define LOG_MAX 3

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
} log_level;

extern log_level global_level; // Current level set using set-log-verbosity
extern UART_HandleTypeDef huart1; // UART where to send logs

/**
 * @brief Log a message if level is greater than the global level
 * @param level Level to log
 * @param fmt String format
 * @param ... Arguments to pass to printf
 */
void gap_log(log_level level, const char *fmt, ...);

/**
 * @brief Log gyroscope data in INFO level with X... Y... Z... format
 * @param data Gyroscope data to log
 */
void gap_log_gyro(GyroData *data);

#endif /* INC_LOGGER_H_ */
