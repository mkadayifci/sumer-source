/*
 * Command_Processor.c
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "command_processor.h"
#include "serial_port.h"
#include "sumer_clock.h"
#include "bluenrg1_stack.h"
#include "app_state.h"
#include "flash_service.h"


uint8_t command_buffer[COMMAND_BUFFER_SIZE];
uint8_t buffer_position=0;

void command_processor_add_to_buffer(uint8_t *receiveBuffer, uint8_t length) {

	for(int i = 0 ;i<length;i++){
		command_buffer[buffer_position]=receiveBuffer[i];
		buffer_position++;
		if(buffer_position > COMMAND_BUFFER_SIZE-1){
			buffer_position=0;
		}
	}
}


void command_processor_switch_to_firmware_upgrade_mode(void){

}


void command_processor_parse_buffer(){
	if(buffer_position<5){//Minimum buffer position to a valid message; 3 Start byte, 1 length, 1 command
		return;
	}

	for(int i = 0;i<buffer_position;i++){
		if(command_buffer[i]==COMMAND_START_SEQ_1){
			if(i+3< buffer_position){ //May contains SEQ 2 SEQ 3 And Length
				if(	command_buffer[i+1]==COMMAND_START_SEQ_2 &&
					command_buffer[i+2]==COMMAND_START_SEQ_3){
					uint8_t command_length =command_buffer[i+3];
					if(i+3+command_length<buffer_position) //Whole Message Is Here
					{
						uint8_t command_to_process[command_length];
						for(int j = 0;j<command_length;j++){
							command_to_process[j]=command_buffer[i+4+j];
						}

						command_processor_process_command((uint8_t *)&command_to_process,command_length);
						command_processor_shrink_buffer(i+4+command_length);

						break;

					}
				}
			}
		}
	}
}


static void command_processor_shrink_buffer(uint8_t position_to_move){
	for(int i =0;i<buffer_position-position_to_move;i++){
		command_buffer[i]=command_buffer[i+position_to_move];

	}
	buffer_position=buffer_position-position_to_move;
}

void command_processor_process_command(uint8_t *receiveBuffer, uint8_t length) {
	switch (receiveBuffer[0]) {
	case COMMAND_GET_VERSION:
		command_processor_send_version_response();
		break;
	case COMMAND_GET_PAGE_LIST:
		command_processor_get_pagelist_response();
		break;
	case COMMAND_SET_TIME:
		command_processor_set_time(receiveBuffer[1],receiveBuffer[2],receiveBuffer[3],receiveBuffer[4],receiveBuffer[5],receiveBuffer[6]);
		break;
	case COMMAND_GET_TIME:
		command_processor_get_time_response();
		break;
	case COMMAND_SCRIBE_GET_WRITTEN_PAGE_COUNT:
		command_processor_send_total_written_page_count();
		break;
	}
}

void command_processor_send_version_response(void) {

	uint8_t response[] = {
			COMMAND_START_SEQ_1,
			COMMAND_START_SEQ_2,
			COMMAND_START_SEQ_3,
			3,
			COMMAND_GET_VERSION,
			FIRMWARE_VERSION_MAJOR,
			FIRMWARE_VERSION_MINOR};
	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));


}



void command_processor_get_pagelist_response(void) {
	uint8_t counter=0;

	for (int i = 0; i < 100; i++) {


		uint8_t* buffer;
		storage_get_page_metadata(i,buffer);


		uint8_t response[] = {
		COMMAND_START_SEQ_1,
		COMMAND_START_SEQ_2,
		COMMAND_START_SEQ_3, 9,
		COMMAND_GET_PAGE_LIST, buffer[0], buffer[1], buffer[2], buffer[3],
		buffer[4], buffer[5],buffer[6], buffer[7] };

		send_data_over_ble_serial((uint8_t*) &response, sizeof(response));

		flush_ble_serial_buffer();

		while (APP_FLAG(TX_BUFFER_FULL)) {
			BTLE_StackTick();
		}

	}
}




void command_processor_send_total_written_page_count(void) {
	uint8_t response[] = {
			COMMAND_START_SEQ_1,
			COMMAND_START_SEQ_2,
			COMMAND_START_SEQ_3,
			3,
			COMMAND_SCRIBE_GET_WRITTEN_PAGE_COUNT,
			0x24, //H
			0x01 //L
			};

	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}



void command_processor_set_time(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second) {

	SumerDateTime time_to_set;
	time_to_set.year=year;
	time_to_set.month=month;
	time_to_set.day=day;
	time_to_set.hour=hour;
	time_to_set.minute=minute;
	time_to_set.second=second;


	sumer_clock_set_time( time_to_set);
}


void command_processor_get_time_response(){


	SumerDateTime device_time = sumer_clock_read_time();

	uint8_t response[] = {
							COMMAND_START_SEQ_1,
							COMMAND_START_SEQ_2,
							COMMAND_START_SEQ_3,
							7,
							COMMAND_GET_TIME,
							device_time.year,
							device_time.month,
							device_time.day,
							device_time.hour,
							device_time.minute,
							device_time.second };


	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));

}
