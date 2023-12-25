/*
 * Command_Processor.h
 *
 *  Created on: Jul 13, 2023
 *      Author: mkadayifci
 */
#include "stdint.h"
#include "hal_types.h"

#ifndef INC_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSOR_H_

#define COMMAND_BUFFER_SIZE	0x64

#define ASCII_CARRIAGE_RETURN	0x0D
#define ASCII_LINE_FEED			0x0A



#define COMMAND_START_SEQ_1		0xFD
#define COMMAND_START_SEQ_2		0x02
#define COMMAND_START_SEQ_3		0xB2

typedef struct {
	uint32_t log_epoch;
	uint32_t page_base_address;
	uint16_t page_count;
	uint16_t seismic_log_group_id;
	uint8_t log_index;
	uint8_t total_log_count;
} log_metadata_t;




#define COMMAND_GET_VERSION						0x41
#define COMMAND_DEBUG_PRINT						0x42
#define COMMAND_GET_TIME						0x43
#define COMMAND_SET_TIME						0x44


#define COMMAND_GET_SEISMIC_LOG_LIST			0x47
#define COMMAND_SWITCH_TO_FIRMWARE_UPGRADE		0x48
#define COMMAND_SWITCH_TO_SEISMIC_DEMO_MODE		0x49
#define COMMAND_SET_SEISMIC_LOG_MODE			0x50
#define COMMAND_GET_SEISMIC_LOG_MODE			0x51
#define COMMAND_GET_SEISMIC_LOG_DETAIL			0x52
#define COMMAND_PAGE_TRANSFER_METADATA			0x53
#define COMMAND_PAGE_CHUNK						0x54
#define COMMAND_GET_DEVICE_SERIAL_NUMBER		0x55
#define COMMAND_FORMAT_FLASH					0x56
#define COMMAND_SET_SEISMIC_ACTIVITY_THRESHOLD	0x58
#define COMMAND_GET_SEISMIC_ACTIVITY_THRESHOLD	0x59
#define COMMAND_SET_SEISMIC_TIME_ACTIVITY		0x5A
#define COMMAND_GET_SEISMIC_TIME_ACTIVITY		0x5B
#define COMMAND_ERASE_SEISMIC_LOG_SECTORS		0x5C


#define FIRMWARE_VERSION_MAJOR	 	0x01
#define FIRMWARE_VERSION_MINOR		0x04

void command_processor_sesimic_log_detail_response(uint8_t * receiveBuffer);
void command_processor_fill_seismic_log_metadata(log_metadata_t * seismic_log_metadata_array);
void command_processor_set_seismic_log_list_total_counts(log_metadata_t * seismic_log_metadata_array);
void command_processor_set_seismic_log_mode_response(uint8_t new_log_mode);
void command_processor_sesimic_log_mode_response(void);
void command_processor_send_version_response(void);
void command_processor_get_device_serial_number_response(void);
void command_processor_send_seismic_demo_response(void);
void command_processor_get_seismic_log_list_response(void);
void command_processor_switch_to_firmware_upgrade_mode(void);
void command_processor_send_total_written_page_count(void);
void command_processor_set_time(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
void command_processor_get_time_response();
static void command_processor_shrink_buffer(uint8_t position_to_move);
void command_processor_sesimic_activity_threshold_response(void);
void command_processor_sesimic_time_threshold_response(void);
void command_processor_set_seismic_time_threshold_response(uint8_t time_threshold_value);
void command_processor_set_seismic_activity_threshold_response(uint8_t threshold_value);



void command_processor_parse_buffer();
void command_processor_add_to_buffer(uint8_t *receiveBuffer, uint8_t length);
void command_processor_process_command(uint8_t* receiveBuffer,uint8_t length);
void command_processor_format_flash_response(void);

#endif /* INC_COMMAND_PROCESSOR_H_ */
