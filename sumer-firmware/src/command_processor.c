/*
 * Command_Processor.c
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "command_processor.h"
#include "serial_port.h"





void command_processor_process_command(uint8_t *receiveBuffer, uint8_t length) {
	switch (receiveBuffer[0]) {
	case COMMAND_GET_VERSION:
		command_processor_send_version_response();
		break;
	case COMMAND_SCRIBE_GET_WRITTEN_PAGE_COUNT:
		command_processor_send_total_written_page_count();
		break;
	}
}


void command_processor_send_version_response(void) {
	uint8_t response[] = { COMMAND_GET_VERSION, FIRMWARE_VERSION_MAJOR,
			FIRMWARE_VERSION_MINOR };
	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}
void command_processor_send_total_written_page_count(void) {
	uint8_t response[] = {
			COMMAND_SCRIBE_GET_WRITTEN_PAGE_COUNT,
			0x24, //H
			0x01,
			ASCII_LINE_FEED  };//L
	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}
