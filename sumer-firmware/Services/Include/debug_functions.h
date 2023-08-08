/*
 * Debug_Functions.h
 *
 *  Created on: Jul 11, 2023
 *      Author: mkadayifci
 */
#include "stdint.h"

#ifndef INC_DEBUG_FUNCTIONS_H_
#define INC_DEBUG_FUNCTIONS_H_

#define MESSAGE_LEVEL_ERROR  	0x00
#define MESSAGE_LEVEL_WARNING  	0x01
#define MESSAGE_LEVEL_INFO  	0x02
#define MESSAGE_LEVEL_VERBOSE  	0x03

#define DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY		0x01
#define DEBUG_ACCELEROMETER_CATEGORY		0x02


#define DEBUG_GATT_INIT_ERROR				0x00
#define DEBUG_GATT_INIT_SUCCESS				0x01
#define DEBUG_GAP_INIT_ERROR				0x02
#define DEBUG_GAP_INIT_SUCCESS				0x03
#define DEBUG_GATT_UPDATE_CHAR_ERROR		0x04
#define DEBUG_GATT_UPDATE_CHAR_SUCCESS		0x05
#define DEBUG_SETTING_BDADDR_ERROR			0x05

#define DEBUG_MOVEMENT_DETECTED				0x00


#define DEBUG_SPACE_HOLDER					0x00

void debug(uint8_t message_type, uint8_t message, uint8_t message_secondary);
void debug_param(uint8_t message_type, uint8_t message,uint8_t message_secondary,uint8_t additional_data);

#endif /* INC_DEBUG_FUNCTIONS_H_ */


