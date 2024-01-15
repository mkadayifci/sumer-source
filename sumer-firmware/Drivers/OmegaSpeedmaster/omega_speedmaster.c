/*
 * omega_speedmaster.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#include "omega_speedmaster.h"
#include "sumer_spi_driver.h"
#include "time.h"

void omega_speedmaster_set_time(OmegaDateTime_t time_to_set)
{
	time_to_set.year = omega_speedmaster_bin2bcd(time_to_set.year);
	time_to_set.month = omega_speedmaster_bin2bcd(time_to_set.month);// | (sumer_clock_read_single(RTCMTH_REGISTER) & RTC_LEAP_YEAR_BIT ) ;
	time_to_set.day = omega_speedmaster_bin2bcd(time_to_set.day);
	time_to_set.hour = omega_speedmaster_bin2bcd(time_to_set.hour) ;
	time_to_set.minute = omega_speedmaster_bin2bcd(time_to_set.minute);
	time_to_set.second = omega_speedmaster_bin2bcd(time_to_set.second);// | (sumer_clock_read_single(RTCSEC_REGISTER) & RTC_ST_BIT ) ;
	omega_speedmaster_set_register(OMEGA_RTCSEC_REGISTER,0x00);//SET ST to 0
	omega_speedmaster_set_register(OMEGA_RTCYEAR_REGISTER,time_to_set.year);
	omega_speedmaster_set_register(OMEGA_RTCMTH_REGISTER,time_to_set.month);
	omega_speedmaster_set_register(OMEGA_RTCDATE_REGISTER,time_to_set.day);
	omega_speedmaster_set_register(OMEGA_RTCHOUR_REGISTER,time_to_set.hour);
	omega_speedmaster_set_register(OMEGA_RTCMIN_REGISTER,time_to_set.minute);
	omega_speedmaster_set_register(OMEGA_RTCSEC_REGISTER,time_to_set.second|OMEGA_RTC_ST_BIT);//SET ST to 1
}

OmegaDateTime_t omega_speedmaster_read_time()
{
	OmegaDateTime_t ret;
	ret.year= omega_speedmaster_get_register(OMEGA_RTCCONTROL_REGISTER);
	ret.year= omega_speedmaster_get_register(OMEGA_RTCYEAR_REGISTER);
	ret.month= omega_speedmaster_get_register(OMEGA_RTCMTH_REGISTER);
	ret.month = ret.month & (~OMEGA_RTC_LEAP_YEAR_BIT);
	ret.day= omega_speedmaster_get_register(OMEGA_RTCDATE_REGISTER);
	ret.hour= omega_speedmaster_get_register(OMEGA_RTCHOUR_REGISTER);
	ret.hour= ret.hour & (~OMEGA_RTC_HOUR_EXT_BITS);
	ret.minute= omega_speedmaster_get_register(OMEGA_RTCMIN_REGISTER);
	ret.second= omega_speedmaster_get_register(OMEGA_RTCSEC_REGISTER);

	ret.second = ret.second & (~OMEGA_RTC_ST_BIT);
	ret.year=((ret.year >> 4 ) *10) + (ret.year & 0x0F);
	ret.month=((ret.month >> 4 ) *10) + (ret.month & 0x0F);
	ret.day=((ret.day >> 4 ) *10) + (ret.day & 0x0F);
	ret.hour=((ret.hour >> 4 ) *10) + (ret.hour & 0x0F);
	ret.minute = ((ret.minute >> 4) * 10) + (ret.minute & 0x0F);
	ret.second = ((ret.second >> 4) * 10) + (ret.second & 0x0F);
	return ret;
}

long omega_speedmaster_get_epoch(void)
{
	OmegaDateTime_t current_time = omega_speedmaster_read_time();
	struct tm t;
    t.tm_year =  (current_time.year + 2000)-1900;
    t.tm_mon = current_time.month-1;           // Month, where 0 = jan
    t.tm_mday = current_time.day;          // Day of the month
    t.tm_hour = current_time.hour;
    t.tm_min = current_time.minute;
    t.tm_sec = current_time.second;
    t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    return mktime(&t);
}

static uint8_t omega_speedmaster_bin2bcd(uint8_t binary_number)
{
    uint8_t digit;
    uint8_t bcd;
    int i;
    for (i = 0; i < 4; i++)
    {
        digit = binary_number % 10;
        binary_number /= 10;
        bcd |= digit << (i * 4); // Store the 4-bit BCD digit in the result
    }
    return bcd;
}

void omega_speedmaster_init()
{
	omega_speedmaster_set_register(OMEGA_RTCSEC_REGISTER,OMEGA_RTC_ST_BIT); // ST Bit
	omega_speedmaster_set_register(OMEGA_RTCCONTROL_REGISTER,0x00);
}

static uint8_t omega_speedmaster_get_register(uint8_t address)
{
	uint8_t buffer[1];
	sumer_spi_driver_read_data(
			SPI_DEVICE_ID_CLOCK,
					(uint8_t*) &buffer,
					(uint8_t[] ) {
		OMEGA_RTC_READ_OPCODE,
									address },
					2,
					1);
	return buffer[0];
}


static void omega_speedmaster_set_register(uint8_t address,uint8_t value)
{
	sumer_spi_driver_write(SPI_DEVICE_ID_CLOCK, (uint8_t[] ) { OMEGA_RTC_WRITE_OPCODE, address, value }, 3);
}
