/*
 * mpu6050.h
 *
 *  Created on: May 6, 2025
 *      Author: lb
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

// Registers
#define MPU6050_I2C_ADDR 0x68 << 1
#define MPU6050_SMPRT_DIV 0x19
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_FIFO_EN 0x23
#define MPU6050_I2C_MST_CTRL 0x24
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_INT_STATUS 0x3A
#define MPU6050_USER_CTRL 0x6A
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_FIFO_COUNT_H 0x72
#define MPU6050_FIFO_R_W 0x74

// Values
#define ZERO_VALUE 0
#define RESET_VALUE 0b10000000
#define SLEEP_VALUE 0b01000000
#define FIFO_GYRO_VALUE 0b01110000
#define FIFO_ENABLE_VALUE 0b01000000
#define FIFO_RESET_VALUE 0b00000100
#define FIFO_OVERFLOW_VALUE 0b00010000
#define DPLF_VALUE 0b00000100

#define FIFO_OVERFLOW 0b00010000
#define RATE_DIVIDER_VALUE 49
#define MAX_OVERFLOW 5
#define LSB_SENSITIVITY 131

typedef struct GyroData {
	int16_t x;
	int16_t y;
	int16_t z;
} GyroData;

/**
 * @brief Initialize the MPU 6050 gyroscope sensor
 * @return HAL status
 */
HAL_StatusTypeDef MPU6050_Init();

/**
 * @brief Read data from the FIFO and extract x, y and z values
 * @param data Structure where to save FIFO data
 * @return HAL_OK if data have been successfully filled, otherwise status != HAL_OK
 */
HAL_StatusTypeDef MPU6050_GetData(GyroData *data);

/**
 * @brief Handle FIFO overflow interrupt
 */
void MPU6050_Handle_FIFO_Overflow();

/**
 * @brief Disable the MPU 6050 gyroscope sensor
 * @return HAL status
 */
HAL_StatusTypeDef MPU6050_Disable();

#endif /* INC_MPU6050_H_ */
