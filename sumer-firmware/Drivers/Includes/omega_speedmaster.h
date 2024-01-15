/*
 * omega_speedmaster.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef OMEGA_SPEEDMASTER_H_
#define OMEGA_SPEEDMASTER_H_

#include "stdint.h"

#define OMEGA_RTCCONTROL_REGISTER	0x08
#define OMEGA_RTCYEAR_REGISTER	0x07
#define OMEGA_RTCMTH_REGISTER		0x06
#define OMEGA_RTCDATE_REGISTER	0x05
#define OMEGA_RTCHOUR_REGISTER	0x03
#define OMEGA_RTCMIN_REGISTER		0x02
#define OMEGA_RTCSEC_REGISTER		0x01

#define OMEGA_RTC_READ_OPCODE		0x13
#define OMEGA_RTC_WRITE_OPCODE	0x12

#define OMEGA_RTC_LEAP_YEAR_BIT	0x20
#define OMEGA_RTC_ST_BIT			0x80
#define OMEGA_RTC_1224_BIT		0x40
#define OMEGA_RTC_HOUR_EXT_BITS	0xC0



typedef struct
{

	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

} OmegaDateTime_t;

void omega_speedmaster_init();
OmegaDateTime_t omega_speedmaster_read_time();
void omega_speedmaster_set_time(OmegaDateTime_t time_to_set);
static uint8_t omega_speedmaster_get_register(uint8_t address);
static void omega_speedmaster_set_register(uint8_t address,uint8_t value);
static uint8_t omega_speedmaster_bin2bcd(uint8_t binary_number);
long omega_speedmaster_get_epoch(void);



#endif /* OMEGA_SPEEDMASTER_H_ */
