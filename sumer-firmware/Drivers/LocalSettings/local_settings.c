/*
 * local_settings.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */


#include "flash_storage.h"

uint8_t local_settings_get_char_value(uint16_t flash_chip_address)
{
	uint8_t buffer[1];
	flash_storage_read_bytes(flash_chip_address, (uint8_t * )&buffer, 1);
	return buffer[0];
}

void local_settings_set_char_value(uint16_t flash_chip_address,uint8_t value)
{
	uint8_t buffer[1];
	buffer[0] = value;
	flash_storage_write_bytes(flash_chip_address, (uint8_t * )&buffer, 1);
}
