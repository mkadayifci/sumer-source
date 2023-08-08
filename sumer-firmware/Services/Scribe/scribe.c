/*
 * scribe.c
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */
#include "clock.h"

void scribe_start(void){

	uint32_t volatile startTick = Clock_Time();
	delay();
	delay();
	delay();
	delay();
	delay();
	delay();
	delay();
	delay();
	delay();
	uint32_t volatile endTick = Clock_Time();
	delay();
}
