/*
 * Command_Processor.h
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "stdint.h"

#ifndef INC_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSOR_H_


#define COMMAND_GET_VERSION		0x41
#define COMMAND_DEBUG_PRINT		0x42
#define COMMAND_GET_PAGE_LIST	0x41


void SendVersionResponse();
void process_command(uint8_t* receiveBuffer,uint8_t length);

#endif /* INC_COMMAND_PROCESSOR_H_ */
