/*
 * Command_Processor.h
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "stdint.h"

#ifndef INC_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSOR_H_

#define ASCII_CARRIAGE_RETURN	0x0D
#define ASCII_LINE_FEED			0x0A

#define COMMAND_GET_VERSION		0x41
#define COMMAND_DEBUG_PRINT		0x42
#define COMMAND_GET_PAGE_LIST	0x41


#define COMMAND_SCRIBE_GET_WRITTEN_PAGE_COUNT	0x45
#define COMMAND_SCRIBE_GET_WHOLE_PAGE			0x46


#define FIRMWARE_VERSION_MAJOR	 	0x01
#define FIRMWARE_VERSION_MINOR		0x04

void command_processor_send_version_response(void);
void command_processor_send_total_written_page_count(void);

void command_processor_process_command(uint8_t* receiveBuffer,uint8_t length);

#endif /* INC_COMMAND_PROCESSOR_H_ */
