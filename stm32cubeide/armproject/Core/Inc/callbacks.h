/*
 * callbacks.h
 *
 *  Created on: May 5, 2025
 *      Author: lb
 */

#ifndef INC_CALLBACKS_H_
#define INC_CALLBACKS_H_

#include <gapcom_core.h>

void ping_callback(gapcom_handle_t *handle, const void *proto_msg);
void set_log_verbosity_callback(gapcom_handle_t *handle, const void *proto_msg);

#endif /* INC_CALLBACKS_H_ */
