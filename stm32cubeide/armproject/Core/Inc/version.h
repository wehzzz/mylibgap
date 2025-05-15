/*
 * version.h
 *
 *  Created on: May 11, 2025
 *      Author: mleve
 */

#ifndef INC_VERSION_H_
#define INC_VERSION_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define WORDS_NUMBER 3
#define SECTOR11_START 0x080E0000
#define EMPTY_ADDRESS 0xffffffff

/**
 * @brief Write version into flash memory
 * @param data Pointer to an array of WORDS_NUMBER of uint32_t
 * @return HAL Status
 */
HAL_StatusTypeDef flash_write_version(uint32_t *data);

/**
 * @brief Read version from flash memory
 * @param data Pointer to an allocated array of length WORDS_NUMBER
 * @return true if data has been found, false otherwise
 */
bool flash_read_version(uint32_t* data);

#endif /* INC_VERSION_H_ */
