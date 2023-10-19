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

static long scribe_start_time=0;
static long cooldown_start_time=0;

void scribe_start(void)
{
	scribe_start_time=sumer_clock_get_epoch();
	scribe_set_scribe_mode();
	accelerometer_set_fifo_to_stream_mode();
}

void scribe_stop(void)
{
	cooldown_start_time=sumer_clock_get_epoch();
	accelerometer_clear_fifo_stream_mode();
	APP_FLAG_CLEAR(SCRIBE_MODE);
	APP_FLAG_SET(SCRIBE_COOLDOWN);
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
			if(local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED)==1)
			{
				accelerometer_sleep_and_enable_interrupt();
			}
		}

	}

}





void scribe_write_seismic_activity_page(void)
{
	uint8_t pBuffer[256];
	accelerometer_read_FIFO(&pBuffer, 256);
	storage_write_acceleration_page(&pBuffer);

}

void scribe_set_scribe_mode(void)
{
	APP_FLAG_SET(SCRIBE_MODE);
	//SET Scribe Start Time
}
