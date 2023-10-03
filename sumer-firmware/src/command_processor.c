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
#include "accelerometer.h"
#include "bluenrg_x_device.h"
#include "local_settings.h"
#include "scribe.h"


uint8_t command_buffer[COMMAND_BUFFER_SIZE];
uint8_t buffer_position=0;

void command_processor_delay(){
	for (uint32_t i = 0; i < 100000; i++)__NOP();
}

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
	case COMMAND_SWITCH_TO_SEISMIC_DEMO_MODE:
		command_processor_send_seismic_demo_response();
		break;
	case COMMAND_SET_SEISMIC_LOG_MODE:
		local_settings_set_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED,receiveBuffer[1]);
		if(receiveBuffer[1]>0 && !APP_FLAG(SCRIBE_COOLDOWN)){
			accelerometer_sleep_and_enable_interrupt();
		}


		command_processor_sesimic_log_mode_response();
		break;
	case COMMAND_GET_SEISMIC_LOG_MODE:
		command_processor_sesimic_log_mode_response();
		break;




	}
}



void command_processor_sesimic_log_mode_response(void) {

	uint8_t is_seismic_log_enabled=local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED);
	uint8_t response[] = {
			COMMAND_START_SEQ_1,
			COMMAND_START_SEQ_2,
			COMMAND_START_SEQ_3,
			2,
			COMMAND_GET_SEISMIC_LOG_MODE,
			is_seismic_log_enabled
			};

	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));



}


void command_processor_send_seismic_demo_response(void) {

	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL,0x13);
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL,0x22);

	uint32_t start_time_epoch=sumer_clock_get_epoch();

	while(sumer_clock_get_epoch() - start_time_epoch <15 ){
		uint8_t x_data_H=accelerometer_spi_read_single(ADXL362_REG_XDATA_H);
		uint8_t x_data_L=accelerometer_spi_read_single(ADXL362_REG_XDATA_L);
		uint8_t y_data_H=accelerometer_spi_read_single(ADXL362_REG_YDATA_H);
		uint8_t y_data_L=accelerometer_spi_read_single(ADXL362_REG_YDATA_L);
		uint8_t z_data_H=accelerometer_spi_read_single(ADXL362_REG_ZDATA_H);
		uint8_t z_data_L=accelerometer_spi_read_single(ADXL362_REG_ZDATA_L);

		uint8_t response[] = {
				COMMAND_START_SEQ_1,
				COMMAND_START_SEQ_2,
				COMMAND_START_SEQ_3,
				7,
				COMMAND_SWITCH_TO_SEISMIC_DEMO_MODE,
				x_data_H,
				x_data_L,
				y_data_H,
				y_data_L,
				z_data_H,
				z_data_L

		};

		send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
		flush_ble_serial_buffer();

		while (APP_FLAG(TX_BUFFER_FULL)) {
			BTLE_StackTick();
		}

		command_processor_delay();
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
	log_metadata_t seismic_log_metadata_array[255]={0};
	command_processor_fill_seismic_log_metadata(&seismic_log_metadata_array);

	for(int i = 0 ;i<255;i++){

		if(seismic_log_metadata_array[i].log_epoch>0x00){
			uint8_t response[] = {
					COMMAND_START_SEQ_1,
					COMMAND_START_SEQ_2,
					COMMAND_START_SEQ_3,
					13,
					COMMAND_GET_PAGE_LIST,
					seismic_log_metadata_array[i].page_base_address & 0xFF,
					seismic_log_metadata_array[i].page_base_address >> 8 & 0xFF,
					seismic_log_metadata_array[i].page_base_address >> 16 & 0xFF,
					seismic_log_metadata_array[i].page_base_address >> 24 & 0xFF,
					seismic_log_metadata_array[i].log_epoch & 0xFF,
					seismic_log_metadata_array[i].log_epoch >> 8 & 0xFF,
					seismic_log_metadata_array[i].log_epoch >> 16 & 0xFF,
					seismic_log_metadata_array[i].log_epoch >> 24 & 0xFF,
					seismic_log_metadata_array[i].page_count & 0xFF,
					seismic_log_metadata_array[i].page_count >> 8 & 0xFF,
					seismic_log_metadata_array[i].log_index,
					seismic_log_metadata_array[i].total_log_count,
					};


			send_data_over_ble_serial((uint8_t*) &response, sizeof(response));

			flush_ble_serial_buffer();

			while (APP_FLAG(TX_BUFFER_FULL)) {
				BTLE_StackTick();
			}
		}
	}
}

void command_processor_fill_seismic_log_metadata(log_metadata_t * seismic_log_metadata_array)
{

	uint8_t metadata_buffer[8];
	uint32_t previous_page_epoch=0;
	uint8_t retIndex=0;
	uint8_t lastSeismicLogStartIndex=0;
	for(uint16_t i = 0;i<STORAGE_TOTAL_ACCELARATION_LOG_PAGE_COUNT;i++){
		storage_get_page_metadata(i,&metadata_buffer);
		uint32_t page_epoch =  	(uint32_t)metadata_buffer[0] |
	             	 	 	 	(uint32_t)metadata_buffer[1] << 8 |
								(uint32_t)metadata_buffer[2] << 16 |
								(uint32_t)metadata_buffer[3] << 24;

		if(abs(previous_page_epoch - page_epoch)>10 && //New Seismic Log Page Founded
			page_epoch != UINT32_MAX &&
			page_epoch != 0 ){
				seismic_log_metadata_array[retIndex].log_epoch = page_epoch;
				seismic_log_metadata_array[retIndex].page_base_address = (i * 8 * 256) + STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;

				if(retIndex>0){
					if(abs(seismic_log_metadata_array[retIndex].log_epoch - seismic_log_metadata_array[retIndex].log_epoch)<SCRIBE_LOG_PERIOD_IN_SEC){ //It is a rollover log

					}
				}

				retIndex++;



		}
		previous_page_epoch=page_epoch;
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
