/*
 * callbacks.c
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */
#include "callbacks.h"
#include <gapcom/gapcom_server.h>
#include "stm32f4xx_hal.h"

void ping_callback(gapcom_handle_t *handle, const void *proto_msg) {
	UNUSED(proto_msg);
	gapcom_respond_ping(handle, GAPErrorCode_GAP_OK);
}
