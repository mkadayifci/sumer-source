/*
 * Command_Processor.c
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "command_processor.h"
#include "serial_port.h"



uint8_t FIRMWARE_VERSION_MAJOR	= 0x01;
uint8_t FIRMWARE_VERSION_MINOR	= 0x04;

void process_command(uint8_t *receiveBuffer, uint8_t length) {
	switch (receiveBuffer[0]) {
	case COMMAND_GET_VERSION:
		SendVersionResponse();

	}
}


void SendVersionResponse() {
	uint8_t response[] = { COMMAND_GET_VERSION, FIRMWARE_VERSION_MAJOR,
			FIRMWARE_VERSION_MINOR };
	SendData((uint8_t * )&response, sizeof(response));
}

