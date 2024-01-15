/*
 * local_settings.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef LOCAL_SETTINGS_H_
#define LOCAL_SETTINGS_H_

#include "stdint.h"

uint8_t local_settings_get_char_value(uint16_t flash_chip_address);
void local_settings_set_char_value(uint16_t flash_chip_address,uint8_t value);



#endif /* LOCAL_SETTINGS_H_ */
