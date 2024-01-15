/*
 * inertial_sensor.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef  INERTIAL_SENSOR_H_
#define  INERTIAL_SENSOR_H_

#include "stdint.h"
#include "hal_types.h"

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

#define ADXL362_SOFT_RESET_VAL			0x52

#define ADXL362_SPI_COMMAND_WRITE		0x0A
#define ADXL362_SPI_COMMAND_READ		0x0B
#define ADXL362_SPI_COMMAND_FIFO_READ	0x0D


enum ADXL_ODR_VALUES {
	ODR_12_5 = 0, ODR_25 = 1, ODR_50 = 2, ODR_100 = 3, ODR_200 = 4, ODR_400 = 5
};

enum ADXL_RANGES {
	RANGE_2G = 0, RANGE_4G = 1, RANGE_8G = 2
};

enum ADXL_MODES {
	ADXL_MODE_STANDBY = 0, ADXL_MODE_MEASURE = 2
};

enum ADXL_NOISE_MODES {
	ADXL_NOISE_MODE_NORMAL = 0, ADXL_NOISE_MODE_LOW = 1, ADXL_NOISE_MODE_ULTRALOW = 2
};

#define INTMAP2_ONLY_FIFO_WATERMARK 					0x04
#define INTMAP1_ONLY_ACTIVITY_INTERRUPT					0x10
#define FIFO_CONTROL_STREAM_AND_SAMPLE_HIGH_BIT 		0x0A

typedef struct
{
    uint8_t ODR : 3;
    uint8_t EXT_SAMPLE : 1;
    uint8_t HALF_BW: 1;
    uint8_t RES: 1;
    uint8_t RANGE: 2;

} adxl_filter_control_t;

typedef struct
{
    uint8_t MEASURE : 2;
    uint8_t AUTOSLEEP : 1;
    uint8_t WAKEUP: 1;
    uint8_t LOW_NOISE: 2;
    uint8_t EXT_CLK: 1;
    uint8_t RES: 1;

} adxl_power_control_t;

void inertial_sensor_delay(void);
void inertial_sensor_init(void);
void inertial_sensor_reset(void);
uint8_t inertial_sensor_get_filter_controls(enum ADXL_ODR_VALUES odr, uint8_t half_bw, enum ADXL_RANGES range);
uint8_t inertial_sensor_get_power_controls(enum ADXL_MODES mode, uint8_t autosleep, uint8_t wakeup, enum ADXL_NOISE_MODES noise_mode);

ErrorStatus inertial_sensor_set_register(uint8_t command, uint8_t value);
uint8_t inertial_sensor_get_register(uint8_t command);
void inertial_sensor_enable_fifo_stream(void);
void inertial_sensor_disable_fifo_stream(void);
void inertial_sensor_init_mcu_external_interrupts(void);
void inertial_sensor_clear_interrupt_bits(void);
void inertial_sensor_disable_activity_interrupt(void);
void inertial_sensor_sleep_and_enable_interrupt(void);
uint16_t inertial_sensor_get_waiting_fifo_records_lenght(void);
void inertial_sensor_read_FIFO(uint8_t *pBuffer, uint16_t length);
void inertial_sensor_clear_interrupt_bits(void);
#endif /* INERTIAL_SENSOR_H_ */
