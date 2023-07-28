
#include <command_processor.h>
#include <debug_functions.h>
#include "serial_port.h"

void debug(uint8_t message_type, uint8_t message, uint8_t message_secondary) {
	uint8_t data[] = { COMMAND_DEBUG_PRINT, message_type, message,
			message_secondary };
	SendData(data, sizeof(data));

}

void debug_param(uint8_t message_type, uint8_t message,uint8_t message_secondary,uint8_t additional_data){


}
