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
#include "stdlib.h"
#include "crc_service.h"
#include "state_manager.h"

uint8_t command_buffer[COMMAND_BUFFER_SIZE];
uint8_t buffer_position=0;

void command_processor_delay(){
	for (uint32_t i = 0; i < 20000; i++)__NOP();
}

void command_processor_add_to_buffer(uint8_t *receiveBuffer, uint8_t length)
{
	for(int i = 0 ;i<length;i++)
	{
		command_buffer[buffer_position]=receiveBuffer[i];
		buffer_position++;
		if(buffer_position > COMMAND_BUFFER_SIZE-1)
		{
			buffer_position=0;
		}
	}
}

void command_processor_switch_to_firmware_upgrade_mode(void)
{

}

void command_processor_parse_buffer()
{
	if(buffer_position<5)//Minimum buffer position to a valid message; 3 Start byte, 1 length, 1 command
	{
		return;
	}
	for(int i = 0;i<buffer_position;i++)
	{
		if(command_buffer[i]==COMMAND_START_SEQ_1)
		{
			if(i+3< buffer_position) //May contains SEQ 2 SEQ 3 And Length
			{
				if(	command_buffer[i+1]==COMMAND_START_SEQ_2 &&
					command_buffer[i+2]==COMMAND_START_SEQ_3)
				{
					uint8_t command_length =command_buffer[i+3];
					if(i+3+command_length<buffer_position) //Whole Message Is Here
					{
						uint8_t command_to_process[command_length];
						for(int j = 0;j<command_length;j++)
						{
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

static void command_processor_shrink_buffer(uint8_t position_to_move)
{
	for(int i =0;i<buffer_position-position_to_move;i++)
	{
		command_buffer[i]=command_buffer[i+position_to_move];
	}
	buffer_position=buffer_position-position_to_move;
}

void command_processor_process_command(uint8_t *receiveBuffer, uint8_t length)
{
	switch (receiveBuffer[0])
	{
		case COMMAND_GET_VERSION:
			command_processor_send_version_response();
			break;
		case COMMAND_GET_SEISMIC_LOG_LIST:
			command_processor_get_seismic_log_list_response();
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
			command_processor_set_seismic_log_mode_response(receiveBuffer[1]);
			break;
		case COMMAND_GET_SEISMIC_LOG_MODE:
			command_processor_sesimic_log_mode_response();
			break;
		case COMMAND_GET_SEISMIC_LOG_DETAIL:
			command_processor_sesimic_log_detail_response(receiveBuffer);
			break;
		case COMMAND_GET_DEVICE_SERIAL_NUMBER:
			command_processor_get_device_serial_number_response();
			break;
		case COMMAND_FORMAT_FLASH:
			command_processor_format_flash_response();
			break;
	}
}

typedef struct
{
	uint8_t session_id;
	uint16_t chunk_count;
	uint32_t page_crc;
	uint16_t page_order_in_session;
	int16_t temp;
	uint32_t page_epoch;
} page_transfer_metadata_t;



#define LOG_PAGE_TRANSFER_CHUNK_SIZE 0x08
static uint8_t seismic_log_detail_session_counter=0;
void command_processor_sesimic_log_detail_response(uint8_t * receiveBuffer)
{
	uint32_t seismic_log_base_address =  	(uint32_t)receiveBuffer[1] |
											(uint32_t)receiveBuffer[2] << 8 |
											(uint32_t)receiveBuffer[3] << 16 |
											(uint32_t)receiveBuffer[4] << 24;
	uint8_t response[] = {
			COMMAND_START_SEQ_1,
			COMMAND_START_SEQ_2,
			COMMAND_START_SEQ_3,
			6,
			COMMAND_GET_SEISMIC_LOG_DETAIL,
			seismic_log_detail_session_counter,
			receiveBuffer[1] ,
			receiveBuffer[2] ,
			receiveBuffer[3] ,
			receiveBuffer[4]
			};
	send_data_over_ble_serial_and_force((uint8_t * )&response, sizeof(response));
	uint8_t page_content_buffer[256];
	uint8_t metadata_buffer[8];
	uint16_t active_page_log_group_id=0;
	int16_t active_page_temprature=0;
	uint16_t previous_page_log_group_id=0;
	uint16_t page_order=0;
	uint32_t active_page_epoch=0;
	uint32_t active_page_address=seismic_log_base_address;
	page_transfer_metadata_t current_page_transfer_metadata;

	while(1)
	{
		page_order++;
		storage_get_page_metadata_by_page_address(active_page_address,metadata_buffer);

		active_page_epoch =	(uint32_t)metadata_buffer[0] |
							(uint32_t)metadata_buffer[1] << 8 |
							(uint32_t)metadata_buffer[2] << 16 |
							(uint32_t)metadata_buffer[3] << 24;

		active_page_log_group_id =  (uint16_t)metadata_buffer[4] |
									(uint16_t)metadata_buffer[5] << 8;
		active_page_log_group_id= active_page_log_group_id & ~(1<<15);

		active_page_temprature = 	(int16_t)metadata_buffer[7] |
									(int16_t)metadata_buffer[6] << 8 ;

		if (previous_page_log_group_id != 0 && (previous_page_log_group_id != active_page_log_group_id)) //Page belongs to different log
		{
			break;
		}
		storage_read_bytes(active_page_address, (uint8_t * )&page_content_buffer, 256);
//		uint32_t crc32_value = crc_service_calculate_crc32c((uint8_t * )&page_content_buffer,256);

		current_page_transfer_metadata.chunk_count=256/LOG_PAGE_TRANSFER_CHUNK_SIZE;
		current_page_transfer_metadata.page_crc = 0;
		current_page_transfer_metadata.page_epoch =active_page_epoch;
		current_page_transfer_metadata.page_order_in_session=page_order;
		current_page_transfer_metadata.session_id=seismic_log_detail_session_counter;
		current_page_transfer_metadata.temp= active_page_temprature;

		uint8_t response[] = {
				COMMAND_START_SEQ_1,
				COMMAND_START_SEQ_2,
				COMMAND_START_SEQ_3,
				16,
				COMMAND_PAGE_TRANSFER_METADATA,
				current_page_transfer_metadata.session_id ,
				current_page_transfer_metadata.chunk_count & 0xFF,
				current_page_transfer_metadata.chunk_count >> 8 & 0xFF,
				current_page_transfer_metadata.page_crc & 0xFF,
				current_page_transfer_metadata.page_crc >> 8 & 0xFF,
				current_page_transfer_metadata.page_crc >> 16 & 0xFF,
				current_page_transfer_metadata.page_crc >> 24 & 0xFF,
				current_page_transfer_metadata.page_order_in_session & 0xFF,
				current_page_transfer_metadata.page_order_in_session >> 8 & 0xFF,
				current_page_transfer_metadata.temp & 0xFF,
				current_page_transfer_metadata.temp >> 8 & 0xFF,
				current_page_transfer_metadata.page_epoch & 0xFF,
				current_page_transfer_metadata.page_epoch >> 8 & 0xFF,
				current_page_transfer_metadata.page_epoch >> 16 & 0xFF,
				current_page_transfer_metadata.page_epoch >> 24 & 0xFF,
				};

		send_data_over_ble_serial_and_force((uint8_t * )&response, sizeof(response));

		for(int i = 0 ;i<current_page_transfer_metadata.chunk_count;i++)
		{
			uint8_t page_chunk_response[] = {
					COMMAND_START_SEQ_1,
					COMMAND_START_SEQ_2,
					COMMAND_START_SEQ_3,
					13,
					COMMAND_PAGE_CHUNK,
					i+1, //Chunk Number
					current_page_transfer_metadata.session_id,
					current_page_transfer_metadata.page_order_in_session & 0xFF,
					current_page_transfer_metadata.page_order_in_session >> 8 & 0xFF,
					page_content_buffer[0 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[1 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[2 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[3 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[4 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[5 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[6 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],
					page_content_buffer[7 + (i * LOG_PAGE_TRANSFER_CHUNK_SIZE)],

					};

			send_data_over_ble_serial_and_force((uint8_t * )&page_chunk_response, sizeof(page_chunk_response));
		}
		active_page_address=storage_get_next_page_address(active_page_address);
		previous_page_log_group_id=active_page_log_group_id;
	}

	//ilk page crc hesapla
//ilk page chunklara başla
//Dön


	//COMMAND_PAGE_CHUNK   HEr sayfa içinde 16bytlık bölümler halinde mesaj atılıacak. data 16byte, chunk_number 1 byte, sessionid 1 byte, session_pageorder 2byte


	seismic_log_detail_session_counter++;
}




void command_processor_set_seismic_log_mode_response(uint8_t new_log_mode)
{

	state_manager_set_is_scribe_mode_enabled(new_log_mode);
	if(new_log_mode && !APP_FLAG(SCRIBE_COOLDOWN)){
		APP_FLAG_SET(WAITING_FOR_ACTIVITY);
		accelerometer_sleep_and_enable_interrupt();
	}
	command_processor_sesimic_log_mode_response();
}



void command_processor_get_device_serial_number_response(void) {
	uint8_t response[] = {
			COMMAND_START_SEQ_1,
			COMMAND_START_SEQ_2,
			COMMAND_START_SEQ_3,
			5,
			COMMAND_GET_DEVICE_SERIAL_NUMBER,
			0x34,//Serial Number Byte1 H
			0x08,
			0xAB,
			0x0F,//Serial Number Byte4 L
			};

	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}


void command_processor_format_flash_response(void)
{

	storage_format_flash_chip();
	storage_use_256_byte_page();
	uint8_t response[] = {
				COMMAND_START_SEQ_1,
				COMMAND_START_SEQ_2,
				COMMAND_START_SEQ_3,
				2,
				COMMAND_FORMAT_FLASH,
				1
				};

	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}

void command_processor_sesimic_log_mode_response(void) {

	uint8_t is_seismic_log_enabled= state_manager_is_scribe_mode_enabled();
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


void command_processor_send_seismic_demo_response(void)
{
	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL,0x17);
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL,0x22);
	uint32_t start_time_epoch=sumer_clock_get_epoch();

	while(sumer_clock_get_epoch() - start_time_epoch <20 )
	{
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
		send_data_over_ble_serial_and_force((uint8_t * )&response, sizeof(response));
		command_processor_delay();
	}
}

void command_processor_send_version_response(void)
{
	uint8_t response[] = {
							COMMAND_START_SEQ_1,
							COMMAND_START_SEQ_2,
							COMMAND_START_SEQ_3,
							3,
							COMMAND_GET_VERSION,
							FIRMWARE_VERSION_MAJOR,
							FIRMWARE_VERSION_MINOR
						};
	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}

void command_processor_get_seismic_log_list_response(void)
{
	uint8_t counter=0;
	log_metadata_t seismic_log_metadata_array[255]={0};
	command_processor_fill_seismic_log_metadata((log_metadata_t * )&seismic_log_metadata_array);
	command_processor_set_seismic_log_list_total_counts((log_metadata_t * )&seismic_log_metadata_array);

	for(int i = 0 ;i<255;i++)
	{
		if(seismic_log_metadata_array[i].log_epoch>0x00)
		{
			uint8_t response[] = {
					COMMAND_START_SEQ_1,
					COMMAND_START_SEQ_2,
					COMMAND_START_SEQ_3,
					13,
					COMMAND_GET_SEISMIC_LOG_LIST,
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
			send_data_over_ble_serial_and_force((uint8_t*) &response, sizeof(response));
		}
	}
}

void command_processor_set_seismic_log_list_total_counts(log_metadata_t * seismic_log_metadata_array)
{
	uint8_t total_log_count = 0;
	for (int i = 0; i < 255; i++)
	{
		if (seismic_log_metadata_array[i].log_epoch > 0x00)
		{
			total_log_count++;
		}
	}
	for (int i = 0; i < 255; i++)
	{
		if (total_log_count > 0 && seismic_log_metadata_array[i].log_epoch > 0x00)
		{
			seismic_log_metadata_array[i].total_log_count = total_log_count;
		}
	}
}

void command_processor_fill_seismic_log_metadata(log_metadata_t * seismic_log_metadata_array)
{
	uint8_t metadata_buffer[8];
	uint8_t retIndex=0;
	uint8_t lastSeismicLogStartIndex=0;
	uint16_t pageCounter =0;

	for(uint16_t i = 0;i<STORAGE_TOTAL_ACCELARATION_LOG_PAGE_COUNT;i++)
	{
		storage_get_page_metadata(i,(uint8_t *)&metadata_buffer);

		uint32_t page_epoch =  	(uint32_t)metadata_buffer[0] |
	             	 	 	 	(uint32_t)metadata_buffer[1] << 8 |
								(uint32_t)metadata_buffer[2] << 16 |
								(uint32_t)metadata_buffer[3] << 24;

		uint16_t seismic_log_group_id = (uint16_t)metadata_buffer[4] |
 	 	 	 							(uint16_t)metadata_buffer[5] << 8 ;

		uint8_t is_first_page_of_seiecmic_log_group = (seismic_log_group_id & (1<<15))>0;
		seismic_log_group_id=  seismic_log_group_id & ~(1<<15);

		if (is_first_page_of_seiecmic_log_group) //New Seismic Log Page Founded
		{
			if (page_epoch != UINT32_MAX && page_epoch != 0) {
				seismic_log_metadata_array[retIndex].log_epoch = page_epoch;
				seismic_log_metadata_array[retIndex].page_base_address = (i * 256) + STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
				seismic_log_metadata_array[retIndex].log_index=retIndex+1;
				seismic_log_metadata_array[retIndex].seismic_log_group_id= seismic_log_group_id;

			}

			if (retIndex>0) {
				seismic_log_metadata_array[retIndex - 1].page_count = pageCounter;
				pageCounter = 0;
			}

			retIndex++;
		}
		pageCounter++;

	}


	if (retIndex > 2) { //There is at least 2 records
		uint16_t last_index=retIndex - 2;

		if ( seismic_log_metadata_array[last_index].seismic_log_group_id  == seismic_log_metadata_array[0].seismic_log_group_id   )// Is there any rollover record. If true these two records have to merge
		{

			seismic_log_metadata_array[0].log_epoch=seismic_log_metadata_array[last_index].log_epoch;
			seismic_log_metadata_array[0].page_base_address=seismic_log_metadata_array[last_index].page_base_address;
			seismic_log_metadata_array[0].page_count=seismic_log_metadata_array[last_index].page_count+seismic_log_metadata_array[0].page_count;

			//Remove Last Item

			seismic_log_metadata_array[last_index].log_epoch=0;
			seismic_log_metadata_array[last_index].page_base_address=0;
			seismic_log_metadata_array[last_index].log_index=0;
			seismic_log_metadata_array[last_index].page_count=0;
			seismic_log_metadata_array[last_index].seismic_log_group_id=0;
			seismic_log_metadata_array[last_index].total_log_count=0;
		}
	}


}

void command_processor_send_total_written_page_count(void)
{
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

void command_processor_set_time(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
	SumerDateTime time_to_set;
	time_to_set.year=year;
	time_to_set.month=month;
	time_to_set.day=day;
	time_to_set.hour=hour;
	time_to_set.minute=minute;
	time_to_set.second=second;
	sumer_clock_set_time( time_to_set);
}

void command_processor_get_time_response()
{
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
							device_time.second
						};
	send_data_over_ble_serial((uint8_t * )&response, sizeof(response));
}
