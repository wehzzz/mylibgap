/*
 * version.c
 *
 *  Created on: May 11, 2025
 *      Author: mleve
 */

#include "version.h"

uint32_t flash_write_version(uint32_t* data) {
	if (HAL_FLASH_Unlock() != HAL_OK) {
		return HAL_ERROR;
	}

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t err;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_12;
	EraseInitStruct.NbSectors = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &err) != HAL_OK) {
		return HAL_FLASH_GetError();
	}

	for (uint32_t i = 0; i < WORDS_NUMBER; i++) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SECTOR12_START + i * 4, data[i]) != HAL_OK)
		{
			return HAL_FLASH_GetError();
		}
	}
	return HAL_FLASH_Lock() == HAL_OK;
}

void flash_read_version(uint32_t* data){
	for (uint32_t i = 0; i < WORDS_NUMBER; i++) {
		data[i] = *((uint32_t*) SECTOR12_START + i * 4);
	}
}
