/*
 * scribe.c
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */
#include "stdio.h"
#include "bluenrg_x_device.h"
#include "accelerometer.h"
void scribe_start(void){

	accelerometer_set_fifo_to_stream_mode();
}

void scribe_write_data(){
	uint8_t volatile buffer[256];
	accelerometer_read_FIFO((uint8_t * )&buffer, sizeof(buffer));

	__NOP();

}
