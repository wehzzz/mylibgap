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
static uint8_t nb_fifo_overflows = 0;

static HAL_StatusTypeDef mem_write(uint16_t reg, uint8_t data) {
	return HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, reg, 1, &data, 1,
	HAL_MAX_DELAY);
}

static HAL_StatusTypeDef mem_read(uint16_t reg, uint8_t *data, uint16_t size) {
	return HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDR, reg, 1, data, size,
	HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_Init() {
	nb_fifo_overflows = 0;
	HAL_StatusTypeDef status = HAL_OK;

	// Reset
	status |= mem_write(MPU6050_PWR_MGMT_1, RESET_VALUE);
	gap_log(LOG_DEBUG, "Reset %d", status);

	// Wake up
	status |= mem_write(MPU6050_PWR_MGMT_1, ZERO_VALUE);
	gap_log(LOG_DEBUG, "Wake up %d", status);

	// FIFO only for gyroscope
	status |= mem_write(MPU6050_FIFO_EN, FIFO_GYRO_VALUE);
	gap_log(LOG_DEBUG, "FIFO only for gyro %d", status);
	// Enable FIFO
	status |= mem_write(MPU6050_USER_CTRL, FIFO_ENABLE_VALUE);
	gap_log(LOG_DEBUG, "Enable FIFO %d", status);

	// 20Hz : Sample Rate = GyroOutputRate / (1 + SMPLRT_DIV)
	// 1kHz / (1 + 49) = 20 Hz so you put 49 as divider
	status |= mem_write(MPU6050_SMPRT_DIV, RATE_DIVIDER_VALUE);
	gap_log(LOG_DEBUG, "Divide rate %d", status);

	// Disable multi-master I2C
	status |= mem_write(MPU6050_I2C_MST_CTRL, ZERO_VALUE);
	gap_log(LOG_DEBUG, "Disable multi-master %d", status);

	// Enable DPLF = Digital Low Pass Filter
	status |= mem_write(MPU6050_CONFIG, DPLF_VALUE);
	gap_log(LOG_DEBUG, "Enable DPLF %d", status);

	// Enable FIFO overflow interrupt
	status |= mem_write(MPU6050_INT_ENABLE, FIFO_OVERFLOW_VALUE);
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

	// Full Scale Range is default value = 250°/s
	// According to the register map, the corresponding LSB Sensitivity is 131 LSB/°/s
	data->x /= 131;
	data->y /= 131;
	data->z /= 131;

	return HAL_OK;
}

void MPU6050_Handle_FIFO_Overflow() {
	if (!gyro_enable)
		return;
	// Verify it's a FIFO overflow interrupt and clear the flag
	uint8_t status;
	mem_read(MPU6050_INT_STATUS, &status, 1);

	if (status & FIFO_OVERFLOW) { // It's a FIFO overflow
		nb_fifo_overflows++;

		gap_log(LOG_ERROR, "FIFO overflow %d/%d", nb_fifo_overflows, MAX_OVERFLOW);
		// Reset FIFO
		mem_write(MPU6050_USER_CTRL, FIFO_RESET_VALUE | FIFO_ENABLE_VALUE);

		if (nb_fifo_overflows >= MAX_OVERFLOW) {
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
	status |= mem_write(MPU6050_USER_CTRL, FIFO_RESET_VALUE);
	gap_log(LOG_DEBUG, "Disable and reset FIFO %d", status);

	// Reset device and sleep
	status |= mem_write(MPU6050_PWR_MGMT_1, RESET_VALUE | SLEEP_VALUE);
	gap_log(LOG_DEBUG, "Reset device and sleep %d", status);

	return status;
}
