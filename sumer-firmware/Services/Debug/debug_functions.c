
#include <command_processor.h>
#include <debug_functions.h>
#include "serial_port.h"

#define DEBUG_WITHOUT_PARAMS_MESSAGE_LENGTH		0x05

void debug(uint8_t message_type, uint8_t message, uint8_t message_secondary) {

	uint8_t data[] = { 	COMMAND_DEBUG_PRINT,
						message_type,
						message,
						message_secondary,
						ASCII_LINE_FEED };

	send_data_over_ble_serial(data,DEBUG_WITHOUT_PARAMS_MESSAGE_LENGTH);

}

void debug_param(uint8_t message_type, uint8_t message,uint8_t message_secondary,uint8_t additional_data){


}
