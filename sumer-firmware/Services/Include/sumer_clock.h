/*
 * sumer_clock.h
 *
 *  Created on: Sep 14, 2023
 *      Author: mkadayifci
 */

#ifndef SERVICES_INCLUDE_SUMER_CLOCK_H_
#define SERVICES_INCLUDE_SUMER_CLOCK_H_


#define RTCCONTROL_REGISTER	0x08
#define RTCYEAR_REGISTER	0x07
#define RTCMTH_REGISTER		0x06
#define RTCDATE_REGISTER	0x05
#define RTCHOUR_REGISTER	0x03
#define RTCMIN_REGISTER		0x02
#define RTCSEC_REGISTER		0x01

#define RTC_READ_OPCODE		0x13
#define RTC_WRITE_OPCODE	0x12

#define RTC_LEAP_YEAR_BIT	0x20
#define RTC_ST_BIT			0x80
#define RTC_1224_BIT		0x40
#define RTC_HOUR_EXT_BITS	0xC0



typedef struct
{

	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

} SumerDateTime;

void sumer_clock_init();
SumerDateTime sumer_clock_read_time();
void sumer_clock_set_time(SumerDateTime time_to_set);
static uint8_t sumer_clock_read_single(uint8_t address);
static void sumer_clock_write_single(uint8_t address,uint8_t value);
static uint8_t sumer_clock_bin2bcd(uint8_t binary_number);

#endif /* SERVICES_INCLUDE_SUMER_CLOCK_H_ */
