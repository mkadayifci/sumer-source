#include "stdint.h"
#include "hal_types.h"

#ifndef _Accelerometer_
#define _Accelerometer_

#define ADXL362_REG_DEVID_AD		0x00
#define ADXL362_REG_DEVID_MST 		0x01
#define ADXL362_REG_PARTID 			0x02
#define ADXL362_REG_REVID 			0x03
#define ADXL362_REG_XDATA 			0x08
#define ADXL362_REG_YDATA 			0x09
#define ADXL362_REG_ZDATA 			0x0A
#define ADXL362_REG_STATUS 			0x0B
#define ADXL362_REG_FIFO_L 			0x0C
#define ADXL362_REG_FIFO_H 			0x0D
#define ADXL362_REG_XDATA_L 		0x0E
#define ADXL362_REG_XDATA_H 		0x0F
#define ADXL362_REG_YDATA_L 		0x10
#define ADXL362_REG_YDATA_H 		0x11
#define ADXL362_REG_ZDATA_L 		0x12
#define ADXL362_REG_ZDATA_H 		0x13
#define ADXL362_REG_TEMP_L 			0x14
#define ADXL362_REG_TEMP_H 			0x15
#define ADXL362_REG_SOFT_RESET 		0x1F
#define ADXL362_REG_THRESH_ACT_L 	0x20
#define ADXL362_REG_THRESH_ACT_H 	0x21
#define ADXL362_REG_TIME_ACT 		0x22
#define ADXL362_REG_THRESH_INACT_L 	0x23
#define ADXL362_REG_THRESH_INACT_H 	0x24
#define ADXL362_REG_TIME_INACT_L 	0x25
#define ADXL362_REG_TIME_INACT_H 	0x26
#define ADXL362_REG_ACT_INACT_CTL 	0x27
#define ADXL362_REG_FIFO_CTL 		0x28
#define ADXL362_REG_FIFO_SAMPLES 	0x29
#define ADXL362_REG_INTMAP1 		0x2A
#define ADXL362_REG_INTMAP2 		0x2B
#define ADXL362_REG_FILTER_CTL 		0x2C
#define ADXL362_REG_POWER_CTL 		0x2D
#define ADXL362_REG_SELF_TEST 		0x2E


#define ADXL362_SPI_SPEED				8000000

#define ADXL362_POWER_CTRL_STANDBY		0b00
#define ADXL362_POWER_CTRL_MEASURE		0b10


#define ADXL362_NOISE_NORMAL			0b00
#define ADXL362_NOISE_LOW				0b01
#define ADXL362_NOISE_ULTRALOW			0b10

#define ADXL362_SOFT_RESET_VAL			0x52

#define ADXL362_SPI_COMMAND_WRITE		0x0A
#define ADXL362_SPI_COMMAND_READ		0x0B
#define ADXL362_SPI_COMMAND_FIFO_READ	0x0D


#define ADXL362_SET						0b1
#define ADXL362_RESET					0b0


typedef struct
{

	uint8_t MEASURE:2;
	uint8_t AUTOSLEEP:1;
	uint8_t WAKEUP:1;
	uint8_t LOW_NOISE:2;
	uint8_t EXT_CLK:1;
	uint8_t RESERVED:1;




}  POWER_CTRL;




void accelerometer_init();


void delay();
void accelerometer_reset();
void accelerometer_sleep_sensor();
void InitEXTI();
void accelerometer_set_fifo_to_stream_mode(void);
void accelerometer_read_FIFO(uint8_t * pBuffer,uint8_t length);
ErrorStatus accelerometer_spi_write_single(uint8_t command, uint8_t value);
uint8_t accelerometer_spi_read_single(uint8_t command) ;
#endif /* _Accelerometer_ */
