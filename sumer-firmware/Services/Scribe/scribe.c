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

static long scribe_start_time=0;
static long cooldown_start_time=0;
static uint8_t waiting_for_start_page=0;
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

uint8_t scribe_is_log_window_over(void)
{
	return ( sumer_clock_get_epoch()-scribe_start_time ) > SCRIBE_LOG_PERIOD_IN_SEC;
}

uint8_t scribe_is_in_scribe_mode(void)
{
	return 	APP_FLAG(SCRIBE_MODE);
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
	storage_write_acceleration_page((uint8_t * )&scribe_accelerometer_FIFO_buffer,waiting_for_start_page);
	waiting_for_start_page=0;
}

void scribe_set_scribe_mode(void)
{
	scribe_start_time=sumer_clock_get_epoch();
	APP_FLAG_SET(SCRIBE_MODE);
}
