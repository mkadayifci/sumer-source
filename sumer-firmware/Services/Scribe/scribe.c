/*
 * scribe.c
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */
#include "scribe.h"
#include "stdio.h"
#include "bluenrg_x_device.h"
#include "accelerometer.h"
#include "app_state.h"
#include "flash_service.h"
#include "local_settings.h"
#include "BlueNRG1_gpio.h"
#include "state_manager.h"
#include "math.h"

static long scribe_start_time=0;
static long cooldown_start_time=0;
static uint8_t waiting_for_start_page=0;
static uint8_t scribe_is_false_positive_val = 0;
void scribe_start(void)
{
	APP_FLAG_CLEAR(WAITING_FOR_ACTIVITY);
	storage_resume_deep_sleep_mode();
	scribe_set_scribe_mode();
	waiting_for_start_page=1;
	accelerometer_set_fifo_to_stream_mode();
}

void scribe_stop(void)
{
	cooldown_start_time=sumer_clock_get_epoch();
	accelerometer_clear_fifo_stream_mode();
	APP_FLAG_CLEAR(SCRIBE_MODE);
	APP_FLAG_SET(SCRIBE_COOLDOWN);
	storage_enter_deep_sleep_mode();
}
void scribe_stop_without_cooldown(void)
{
	scribe_stop();
	cooldown_start_time-=SCRIBE_COOLDOWN_PERIOD_IN_SEC;
}


uint8_t scribe_is_log_window_over(void)
{
	return ( sumer_clock_get_epoch()-scribe_start_time ) > SCRIBE_LOG_PERIOD_IN_SEC;
}

uint8_t scribe_is_in_scribe_mode(void)
{
	return 	APP_FLAG(SCRIBE_MODE);
}

uint8_t scribe_is_false_positive(void)
{
	return 	scribe_is_false_positive_val;
}
uint8_t scribe_is_in_cooldown_period(void)
{
	if (cooldown_start_time == 0)
	{
		return 0;
	}
	long current_epoch_time = sumer_clock_get_epoch();
	if ((current_epoch_time - cooldown_start_time ) < SCRIBE_COOLDOWN_PERIOD_IN_SEC)
	{
		return 1;
	}
	return 0;
}

void scribe_tick(void)
{
	if(APP_FLAG(SCRIBE_COOLDOWN))
	{
		if(!scribe_is_in_cooldown_period())
		{
			APP_FLAG_CLEAR(SCRIBE_COOLDOWN);
			if(state_manager_is_scribe_mode_enabled()==1)
			{
				APP_FLAG_SET(WAITING_FOR_ACTIVITY);
				accelerometer_sleep_and_enable_interrupt();
			}
		}
	}
}

uint8_t scribe_accelerometer_FIFO_buffer[256];
void scribe_write_seismic_activity_page(void)
{
	accelerometer_read_FIFO((uint8_t * )&scribe_accelerometer_FIFO_buffer, 256);

	if(waiting_for_start_page){
		uint8_t filter_value= scribe_apply_false_positive_filter((uint8_t * )&scribe_accelerometer_FIFO_buffer,256);
		if(filter_value){
			scribe_is_false_positive_val=1;
			//scribe_stop();
		}
	}

	storage_write_acceleration_page((uint8_t * )&scribe_accelerometer_FIFO_buffer,waiting_for_start_page);
	waiting_for_start_page=0;
	scribe_is_false_positive_val=0;
}


void scribe_set_scribe_mode(void)
{
	scribe_start_time=sumer_clock_get_epoch();
	APP_FLAG_SET(SCRIBE_MODE);
}

uint8_t volatile scribe_apply_false_positive_filter(uint8_t * pBuffer, uint16_t length)
{
	int16_t current_x;
	int16_t current_y;
	int16_t current_z;
	int16_t x_array[42] = {0};
	int16_t y_array[42] = {0};
	int16_t z_array[42] = {0};
	double x_std_dev;
	double y_std_dev;
	double z_std_dev;
	double x_threshold = 3.0;
	double y_threshold = 3.0;
	double z_threshold = 4.0;
	uint8_t array_indexer = 0;
	for (uint16_t i = 0; i < 252; i += 6)
	{
		uint8_t signExtensionBits;
		uint8_t highByte;

		highByte=pBuffer[i+1];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_x = (int16_t)(highByte<<8) |  (int16_t)pBuffer[i];


		highByte=pBuffer[i+3];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_y = (int16_t) (highByte<<8) |  (int16_t)pBuffer[i + 2];


		highByte=pBuffer[i+5];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_z = (int16_t)(highByte<<8)| (int16_t)pBuffer[i + 4];


		x_array[array_indexer] = current_x;
		y_array[array_indexer] = current_y;
		z_array[array_indexer] = current_z;
		array_indexer++;
	}
	x_std_dev = scribe_calculate_standart_deviation(x_array,10, 30);
	y_std_dev = scribe_calculate_standart_deviation(y_array,10, 30);
	z_std_dev = scribe_calculate_standart_deviation(z_array,10, 30);

	if (x_std_dev < x_threshold &&
		y_std_dev < y_threshold &&
		z_std_dev < z_threshold)
	{
		return 1;
	}
	return 0;
}

double scribe_calculate_standart_deviation(const int16_t pData[],uint16_t start , uint16_t length )
{
	if (length <= 1)
	{
		return 0.0;
	}

	double mean = 0;
	for (int i = start; i < length + start ; i++)
	{
		mean += pData[i];
	}
	mean /= length;

	double variance = 0.0;
	for (int i = start; i < length + start; i++)
	{
		double diff = pData[i] - mean;
		variance += diff * diff;
	}
	variance /= length;
	double standardDeviation = sqrt(variance);
	return standardDeviation;
}
