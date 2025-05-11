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


	// todo -> check si semver valide

	uint32_t version[3];
	version[0] = char_to_uint32(req->version);
	version[1] = char_to_uint32(req->version + 4);
	version[2] = char_to_uint32(req->version + 8);
	if (flash_write_version(version) == HAL_OK) {
		gapcom_respond_set_version(handle, GAPErrorCode_GAP_OK);
	}
	else {
		gapcom_respond_set_version(handle, GAPErrorCode_GAP_INVALID_VERSION_FORMAT);
	}
}

void get_version_callback(gapcom_handle_t *handle, const void *proto_msg) {
	gap_log(LOG_DEBUG, "GET_VERSION_REQ received");
	UNUSED(proto_msg);

	uint32_t version_tmp[3];
	char version[12];

	flash_read_version(version_tmp);

	//todo check if nothing -> return 0.0.0
	uint32_to_char(version_tmp, version);

	gapcom_respond_get_version(handle, GAPErrorCode_GAP_OK, version);
}
