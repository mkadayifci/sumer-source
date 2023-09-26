/*
 * local_settings.c
 *
 *  Created on: Sep 26, 2023
 *      Author: mkadayifci
 */
#include "flash_service.h"

uint8_t local_settings_get_char_value(uint16_t flash_chip_address){
	uint8_t* buffer[1];
	storage_read_bytes(flash_chip_address, buffer, 1);
	return buffer[0];
}

void local_settings_set_char_value(uint16_t flash_chip_address,uint8_t value){
	uint8_t* buffer[1];
	buffer[0]=value;
	storage_write_bytes(flash_chip_address,buffer,1);
}

