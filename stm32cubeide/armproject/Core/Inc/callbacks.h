/*
 * callbacks.h
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */

#ifndef INC_CALLBACKS_H_
#define INC_CALLBACKS_H_

#include <gapcom_core.h>

/**
 * @brief Function called when PING_REQ is received
 * 		  Always respond GAP_OK
 * @param handle Handle to libgapcom session
 * @param proto_msg Received message
 */
void ping_callback(gapcom_handle_t *handle, const void *proto_msg);

/**
 * @brief Function called when SET_LOG_VERBOSITY_REQ is received
 * 		  Respond GAP_OK if the given verbosity is correct, GAP_INVALID_LOG_VERBOSITY otherwise
 * @param handle Handle to libgapcom session
 * @param proto_msg Received message
 */
void set_log_verbosity_callback(gapcom_handle_t *handle, const void *proto_msg);

/**
 * @brief Function called when SET_VERSION_REQ is received
 * 		  Respond GAP_OK if the given version is a correct semver and was successfully saved in flash memory
 * 		  , GAP_INVALID_LOG_VERBOSITY otherwise
 * @param handle Handle to libgapcom session
 * @param proto_msg Received message
 */
void set_version_callback(gapcom_handle_t *handle, const void *proto_msg);

/**
 * @brief Function called when GET_VERSION_REQ is received
 * 		  Respond GAP_OK if no errors were encountered while retrieving the version, GAP_INVALID_LOG_VERBOSITY otherwise
 * @param handle Handle to libgapcom session
 * @param proto_msg Received message
 */
void get_version_callback(gapcom_handle_t *handle, const void *proto_msg);

#endif /* INC_CALLBACKS_H_ */
