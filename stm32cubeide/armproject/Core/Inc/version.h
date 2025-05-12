/*
 * version.h
 *
 *  Created on: May 11, 2025
 *      Author: mleve
 */

#ifndef INC_VERSION_H_
#define INC_VERSION_H_

#include "stm32f4xx_hal.h"

#define WORDS_NUMBER 3
#define SECTOR12_START 0x08100000

HAL_StatusTypeDef flash_write_version(uint32_t *data);
void flash_read_version(uint32_t* data);

#endif /* INC_VERSION_H_ */
