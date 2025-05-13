/*
 * callbacks.c
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */
#include "callbacks.h"

#include <gapcom/gapcom_server.h>

#include "stm32f4xx_hal.h"
#include "logger.h"
#include "version.h"
#include "utils.h"

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

void set_version_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "SET_VERSION_REQ received");

	const GAPSetVersionReq *req = (const GAPSetVersionReq *)proto_msg;

	if (!is_valid_semver(req->version)) {
		gap_log(LOG_DEBUG, "SET_VERSION_REQ invalid semver");
		gapcom_respond_set_version(handle, GAPErrorCode_GAP_INVALID_VERSION_FORMAT);
		return;
	}

	uint32_t version[3];
	memcpy(version, req->version, sizeof(req->version));
	//version[0] = char_to_uint32(req->version);
	//version[1] = char_to_uint32(req->version + 4);
	//version[2] = char_to_uint32(req->version + 8);

	gap_log(LOG_DEBUG, "SET version[0] = %d", version[0]);
	gap_log(LOG_DEBUG, "SET version[1] = %d", version[1]);
	gap_log(LOG_DEBUG, "SET version[2] = %d", version[2]);

	if (flash_write_version(version) == HAL_OK) {
		gap_log(LOG_DEBUG, "SET_VERSION_REQ write to flash successful");
		gapcom_respond_set_version(handle, GAPErrorCode_GAP_OK);
	}
	else {
		gap_log(LOG_DEBUG, "SET_VERSION_REQ cannot write to flash");
		gapcom_respond_set_version(handle, GAPErrorCode_GAP_INVALID_VERSION_FORMAT);
	}
}

void get_version_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "GET_VERSION_REQ received");
	UNUSED(proto_msg);

	uint32_t version_tmp[3];
	char version[12];

	if (!flash_read_version(version_tmp)) {
		version[0] = '0';
		version[1] = '.';
		version[2] = '0';
		version[3] = '.';
		version[4] = '0';
		version[5] = '\0';
	}
	else {
		gap_log(LOG_DEBUG, "GET version[0] = %d", version_tmp[0]);
		gap_log(LOG_DEBUG, "GET version[1] = %d", version_tmp[1]);
		gap_log(LOG_DEBUG, "GET version[2] = %d", version_tmp[2]);
		//uint32_to_char(version_tmp, version);
		memcpy(version, version_tmp, sizeof(version_tmp));
	}
	gap_log(LOG_DEBUG, "version %s", version);

	gapcom_respond_get_version(handle, GAPErrorCode_GAP_OK, version);
}
