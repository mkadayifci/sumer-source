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

static long scribe_start_time=0;
static long cooldown_start_time=0;

void scribe_start(void){
	scribe_start_time=sumer_clock_get_epoch();
	scribe_set_scribe_mode();
	accelerometer_set_fifo_to_stream_mode();
}
void scribe_stop(void){
	cooldown_start_time=sumer_clock_get_epoch();
	accelerometer_clear_fifo_stream_mode();
	APP_FLAG_CLEAR(SCRIBE_MODE);
	APP_FLAG_SET(SCRIBE_COOLDOWN);
}

void scribe_write_seismic_activity_page(void) {

	uint8_t *buffer[256];
	accelerometer_read_FIFO(buffer, sizeof(buffer));

	//TODO: Get temp and pass it
	storage_write_acceleration_page(buffer, 22);

	if ((sumer_clock_get_epoch() - scribe_start_time) > SCRIBE_LOG_PERIOD_IN_SEC) {
		scribe_stop();
	}
}


void scribe_set_scribe_mode(void){

	APP_FLAG_SET(SCRIBE_MODE);
	//SET Scribe Start Time
}
