#include "SPI_service.h"
#include "sumer_clock.h"
#include "time.h"


void sumer_clock_set_time(SumerDateTime time_to_set){

	//SET ST to 0
	// Convert to BCD

	time_to_set.year = sumer_clock_bin2bcd(time_to_set.year);
	time_to_set.month = sumer_clock_bin2bcd(time_to_set.month);// | (sumer_clock_read_single(RTCMTH_REGISTER) & RTC_LEAP_YEAR_BIT ) ;
	time_to_set.day = sumer_clock_bin2bcd(time_to_set.day);
	time_to_set.hour = sumer_clock_bin2bcd(time_to_set.hour) ;
	time_to_set.minute = sumer_clock_bin2bcd(time_to_set.minute);
	time_to_set.second = sumer_clock_bin2bcd(time_to_set.second);// | (sumer_clock_read_single(RTCSEC_REGISTER) & RTC_ST_BIT ) ;

	sumer_clock_write_single(RTCSEC_REGISTER,0x00);//SET ST to 0

	sumer_clock_write_single(RTCYEAR_REGISTER,time_to_set.year);
	sumer_clock_write_single(RTCMTH_REGISTER,time_to_set.month);
	sumer_clock_write_single(RTCDATE_REGISTER,time_to_set.day);
	sumer_clock_write_single(RTCHOUR_REGISTER,time_to_set.hour);
	sumer_clock_write_single(RTCMIN_REGISTER,time_to_set.minute);
	sumer_clock_write_single(RTCSEC_REGISTER,time_to_set.second|RTC_ST_BIT);//SET ST to 1
}

SumerDateTime sumer_clock_read_time(){

	SumerDateTime ret;


	ret.year= sumer_clock_read_single(RTCCONTROL_REGISTER);
	ret.year= sumer_clock_read_single(RTCYEAR_REGISTER);
	ret.month= sumer_clock_read_single(RTCMTH_REGISTER);
	ret.month = ret.month & (~RTC_LEAP_YEAR_BIT);
	ret.day= sumer_clock_read_single(RTCDATE_REGISTER);
	ret.hour= sumer_clock_read_single(RTCHOUR_REGISTER);
	ret.hour= ret.hour & (~RTC_HOUR_EXT_BITS);


	ret.minute= sumer_clock_read_single(RTCMIN_REGISTER);
	ret.second= sumer_clock_read_single(RTCSEC_REGISTER);
	ret.second = ret.second & (~RTC_ST_BIT);

	ret.year=((ret.year >> 4 ) *10) + (ret.year & 0x0F);
	ret.month=((ret.month >> 4 ) *10) + (ret.month & 0x0F);
	ret.day=((ret.day >> 4 ) *10) + (ret.day & 0x0F);
	ret.hour=((ret.hour >> 4 ) *10) + (ret.hour & 0x0F);
	ret.minute = ((ret.minute >> 4) * 10) + (ret.minute & 0x0F);
	ret.second = ((ret.second >> 4) * 10) + (ret.second & 0x0F);


	return ret;
}

long sumer_clock_get_epoch(void){

	SumerDateTime current_time = sumer_clock_read_time();
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

// function to convert a binary number to BCD format
static uint8_t sumer_clock_bin2bcd(uint8_t binary_number) {
    uint8_t digit;
    uint8_t bcd;
    int i;

    for (i = 0; i < 4; i++) {
        digit = binary_number % 10;
        binary_number /= 10;
        bcd |= digit << (i * 4); // Store the 4-bit BCD digit in the result
    }
    return bcd;
}

void sumer_clock_init(){
	sumer_clock_write_single(RTCSEC_REGISTER,RTC_ST_BIT); // ST Bit
	sumer_clock_write_single(RTCCONTROL_REGISTER,0x00);
}

static uint8_t sumer_clock_read_single(uint8_t address) {
	uint8_t buffer[1];

	spi_service_read_data(
			SPI_DEVICE_ID_CLOCK,
					(uint8_t*) &buffer,
					(uint8_t[] ) {
									RTC_READ_OPCODE,
									address },
					2,
					1);
	return buffer[0];
}


static void sumer_clock_write_single(uint8_t address,uint8_t value) {
	spi_service_write(SPI_DEVICE_ID_CLOCK, (uint8_t[] ) {
			RTC_WRITE_OPCODE, address, value }, 3);
}
