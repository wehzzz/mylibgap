/*
 * callbacks.c
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */
#include "callbacks.h"

#include <gapcom/gapcom_server.h>

#include "stm32f4xx_hal.h"
#include "mpu6050.h"
#include "logger.h"

void ping_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "PING_REQ received");
	UNUSED(proto_msg);
	gapcom_respond_ping(handle, GAPErrorCode_GAP_OK);
}

void set_log_verbosity_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "SET_LOG_VERBOSITY_REQ received");

	const GAPSetLogVerbosityReq *req = (const GAPSetLogVerbosityReq *)proto_msg;
	log_level tmp_level = (log_level)req->verbosity;

	if (tmp_level < 0 || tmp_level > LOG_MAX) {
		gapcom_respond_set_log_verbosity(handle, GAPErrorCode_GAP_INVALID_LOG_VERBOSITY);
	}
	else {
		global_level = tmp_level;
		gapcom_respond_set_log_verbosity(handle, GAPErrorCode_GAP_OK);
	}
}

void set_gyroscope_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "SET_GYROSCOPE received");

	const GAPSetGyroscopeReq *req = (const GAPSetGyroscopeReq*) proto_msg;
	if (req->set) {
		// Enable the gyroscope
		gap_log(LOG_INFO, "Starting the gyroscope");
		if (MPU6050_Init() != HAL_OK) {
			gap_log(LOG_ERROR, "Unable to start the gyroscope");
		}
	} else {
		// Disable the gyroscope
		gap_log(LOG_INFO, "Stopping the gyroscope");
		if (MPU6050_Disable() != HAL_OK) {
			gap_log(LOG_ERROR, "Unable to stop the gyroscope");
		}
	}
	gapcom_respond_set_gyroscope(handle, GAPErrorCode_GAP_OK);
}
