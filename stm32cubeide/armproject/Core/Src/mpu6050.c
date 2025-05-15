/*
 * mpu6050.c
 *
 *  Created on: May 6, 2025
 *      Author: lb
 */
#include "mpu6050.h"

#include <stdbool.h>

#include "logger.h"

static bool gyro_enable = false;

static HAL_StatusTypeDef mem_write(uint16_t reg, uint8_t data) {
	return HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, reg, 1, &data, 1,
	HAL_MAX_DELAY);
}

static HAL_StatusTypeDef mem_read(uint16_t reg, uint8_t *data, uint16_t size) {
	return HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDR, reg, 1, data, size,
	HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_Init() {
	HAL_StatusTypeDef status = HAL_OK;

	// Reset
	status |= mem_write(MPU6050_PWR_MGMT_1, 0b10000000);
	gap_log(LOG_DEBUG, "Reset %d", status);

	// Wake up
	status |= mem_write(MPU6050_PWR_MGMT_1, 0);
	gap_log(LOG_DEBUG, "Wake up %d", status);

	// FIFO only for gyroscope
	status |= mem_write(MPU6050_FIFO_EN, 0b01110000);
	gap_log(LOG_DEBUG, "FIFO only for gyro %d", status);
	// Enable FIFO
	status |= mem_write(MPU6050_USER_CTRL, 0b01000000);
	gap_log(LOG_DEBUG, "Enable FIFO %d", status);

	// 20Hz : Sample Rate = GyroOutputRate / (1 + SMPLRT_DIV)
	// 1kHz / (1 + 49) = 20 Hz so you put 49 as divider
	status |= mem_write(MPU6050_SMPRT_DIV, 49);
	gap_log(LOG_DEBUG, "Divide rate %d", status);

	// Disable multi-master I2C
	status |= mem_write(MPU6050_I2C_MST_CTRL, 0);
	gap_log(LOG_DEBUG, "Disable multi-master %d", status);

	// Enable DPLF = Digital Low Pass Filter
	status |= mem_write(MPU6050_CONFIG, 0b00000100);
	gap_log(LOG_DEBUG, "Enable DPLF %d", status);

	// Enable FIFO overflow interrupt
	status |= mem_write(MPU6050_INT_ENABLE, 0b00010000);
	gap_log(LOG_DEBUG, "Enable FIFO overflow interrupt %d", status);

	if (status == HAL_OK) {
		gyro_enable = true;
	}
	return status;
}

HAL_StatusTypeDef MPU6050_GetData(GyroData *data) {
	if (!gyro_enable)
		return HAL_ERROR;

	uint8_t fifo_count_buf[2];
	HAL_StatusTypeDef status;

	// Read FIFO count
	status = mem_read(MPU6050_FIFO_COUNT_H, fifo_count_buf, 2);
	if (status != HAL_OK) {
		gap_log(LOG_DEBUG, "Unable to read fifo count");
		return status;
	}

	uint16_t fifo_count = (fifo_count_buf[0] << 8) | fifo_count_buf[1];
	// Data is in 6 bytes: 2 per axis
	if (fifo_count < 6) {
		// gap_log(LOG_DEBUG, "Not enough data to read %d", fifo_count);
		return HAL_ERROR;
	}

	// Read 1 data so 6 bytes
	uint8_t fifo_data[6];
	status = mem_read(MPU6050_FIFO_R_W, fifo_data, 6);
	if (status != HAL_OK) {
		gap_log(LOG_DEBUG, "Unable to read 6 bytes in FIFO");
		return status;
	}

	// Convert data and store them inside the given pointer
	data->x = (fifo_data[0] << 8) | fifo_data[1];
	data->y = (fifo_data[2] << 8) | fifo_data[3];
	data->z = (fifo_data[4] << 8) | fifo_data[5];

	return HAL_OK;
}

void MPU6050_Handle_FIFO_Overflow() {
	// Verify it's a FIFO overflow interrupt and clear the flag
	uint8_t status;
	mem_read(MPU6050_INT_STATUS, &status, 1);

	if (status & 0b00010000) { // It's a FIFO overflow
		static uint8_t nb_fifo_overflows = 0;
		nb_fifo_overflows++;

		gap_log(LOG_ERROR, "FIFO overflow %d/5", nb_fifo_overflows);
		// Reset FIFO
		mem_write(MPU6050_USER_CTRL, 0b00000100);

		if (nb_fifo_overflows >= 5) {
			nb_fifo_overflows = 0;
			MPU6050_Disable();
			gap_log(LOG_ERROR,
					"Disabling MPU6050, 5 FIFO overflow detected");
		}
	}
}

HAL_StatusTypeDef MPU6050_Disable() {
	gyro_enable = false;
	HAL_StatusTypeDef status = HAL_OK;

	// Disable and reset FIFO
	status |= mem_write(MPU6050_USER_CTRL, 0b01000100);

	// Reset device and sleep
	status |= mem_write(MPU6050_PWR_MGMT_1, 0b11000000);

	return status;
}
