/*
 * local_settings.h
 *
 *  Created on: Sep 26, 2023
 *      Author: mkadayifci
 */

#include "stdint.h"

#ifndef SERVICES_INCLUDE_LOCAL_SETTINGS_H_
#define SERVICES_INCLUDE_LOCAL_SETTINGS_H_


uint8_t local_settings_get_char_value(uint16_t flash_chip_address);
void local_settings_set_char_value(uint16_t flash_chip_address,uint8_t value);


#endif /* SERVICES_INCLUDE_LOCAL_SETTINGS_H_ */


