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

#define MPU6050_I2C_ADDR 0x68 << 1
#define MPU6050_SMPRT_DIV 0x19
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_FIFO_EN 0x23
#define MPU6050_I2C_MST_CTRL 0x24
#define MPU6050_USER_CTRL 0x6A
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_FIFO_COUNT_H 0x72
#define MPU6050_FIFO_R_W 0x74

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
 * @brief Disable the MPU 6050 gyroscope sensor
 * @return HAL status
 */
HAL_StatusTypeDef MPU6050_Disable();


#endif /* INC_MPU6050_H_ */
