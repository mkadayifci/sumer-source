/*
 * sumer_scriber.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef SUMER_SCRIBER_H_
#define SUMER_SCRIBER_H_

#define SUMER_SCRIBER_LOG_PERIOD_IN_SEC		20 	// 	60
#define SUMER_SCRIBER_COOLDOWN_PERIOD_IN_SEC	10 // 	300

#define MAX_SEISMIC_LOG_GROUP_ID	0x7FFF

typedef struct
{
	uint32_t scribe_start_time;
	uint32_t cooldown_start_time;
	uint8_t is_first_page;
} scriber_state_t;



void sumer_scriber_start(void);
void sumer_scriber_stop(void);
void sumer_scriber_stop_without_cooldown(void);
void sumer_scriber_write_log_page(void);
uint8_t sumer_scriber_is_in_cooldown_period(void);
uint8_t sumer_scriber_is_log_window_over(void);
double sumer_scriber_calculate_standart_deviation(const int16_t pData[],uint16_t start , uint16_t length );
uint8_t volatile sumer_scriber_apply_false_positive_filter(uint8_t * pBuffer, uint16_t length);
static uint32_t sumer_scriber_get_next_page_address_from_flash(void);
static void sumer_scriber_set_seismic_log_page_metadata(uint32_t written_page_address,uint32_t time_epoch,uint16_t seismic_log_group_id,uint8_t temp_H,uint8_t temp_L);
static void sumer_scriber_write_last_seismic_log_group_id_to_storage(uint16_t last_seismic_log_group_id);
static uint16_t sumer_scriber_get_last_seismic_log_group_id_from_storage(void);
static void sumer_scriber_set_whole_metadata(uint32_t page_address_to_write);
void sumer_scriber_write_log_page_from_inertial_sensor_fifo(void);
void sumer_scriber_get_page_metadata_by_page_address(uint32_t page_address,uint8_t* buffer);
void sumer_scriber_get_page_metadata(uint16_t page_index,uint8_t * buffer);
static void sumer_scriber_increase_next_page_address(uint32_t current_page_address);
uint32_t sumer_scriber_get_calculated_next_page_address(uint32_t current_page_address);
scriber_state_t * sumer_scriber_get_state(void);
#endif /* SUMER_SCRIBER_H_ */

