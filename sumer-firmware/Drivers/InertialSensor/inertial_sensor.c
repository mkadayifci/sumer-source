/*
 * inertial_sensor.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#include "inertial_sensor.h"
#include "sumer_spi_driver.h"
#include "state_manager.h"

void inertial_sensor_delay(void)
{
	for (uint32_t i = 0; i < 80000; i++)__NOP();
}

void inertial_sensor_init(void)
{
	inertial_sensor_reset();
	inertial_sensor_disable_fifo_stream();
	inertial_sensor_set_register(ADXL362_REG_FILTER_CTL, inertial_sensor_get_filter_controls(ODR_200, 0x01, RANGE_2G));
	inertial_sensor_set_register(ADXL362_REG_POWER_CTL, inertial_sensor_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_ULTRALOW));
	inertial_sensor_set_register(ADXL362_REG_FIFO_CTL, 0x00);
	inertial_sensor_set_register(ADXL362_REG_INTMAP2, 0x00);
	inertial_sensor_set_register(ADXL362_REG_INTMAP1, 0x00);
	inertial_sensor_clear_fifo();
	inertial_sensor_set_register(ADXL362_REG_FIFO_CTL, FIFO_CONTROL_STREAM_AND_SAMPLE_HIGH_BIT);

}

uint8_t inertial_sensor_get_filter_controls(enum ADXL_ODR_VALUES odr,uint8_t half_bw,enum ADXL_RANGES range)
{
	adxl_filter_control_t filter_control={0};
	filter_control.ODR = odr;
	filter_control.EXT_SAMPLE= 0x00;
	filter_control.HALF_BW = half_bw;
	filter_control.RANGE = range;
	return  ((uint8_t *)&filter_control)[0];
}

uint8_t inertial_sensor_get_power_controls(enum ADXL_MODES mode,uint8_t autosleep,uint8_t wakeup,enum ADXL_NOISE_MODES noise_mode)
{
	adxl_power_control_t power_control={0};
	power_control.MEASURE= mode;
	power_control.AUTOSLEEP=autosleep;
	power_control.WAKEUP= wakeup;
	power_control.LOW_NOISE=noise_mode;
	power_control.EXT_CLK=0x00;
	return ((uint8_t *)&power_control)[0];
}

uint16_t inertial_sensor_get_waiting_fifo_records_lenght(void)
{
	uint8_t fifo_samples_L=inertial_sensor_get_register(ADXL362_REG_FIFO_L);
	uint8_t fifo_samples_H=inertial_sensor_get_register(ADXL362_REG_FIFO_H);
	return (uint16_t)fifo_samples_H << 8 | fifo_samples_L;
}

void inertial_sensor_reset(void)
{
	inertial_sensor_set_register(ADXL362_REG_SOFT_RESET,ADXL362_SOFT_RESET_VAL);
	inertial_sensor_delay();
}

ErrorStatus inertial_sensor_set_register(uint8_t command, uint8_t value)
{
	return sumer_spi_driver_write(
							SPI_DEVICE_ID_ADXL,

							(uint8_t[]) {
										ADXL362_SPI_COMMAND_WRITE,
										command,
										value },
							3);
}

uint8_t inertial_sensor_get_register(uint8_t command)
{
	uint8_t buffer[1];
	sumer_spi_driver_read_data(
					SPI_DEVICE_ID_ADXL,
					(uint8_t*) &buffer,
					(uint8_t[] ) {
									ADXL362_SPI_COMMAND_READ,
									command },
					2,
					1);
	return buffer[0];
}


void inertial_sensor_clear_fifo(void)
{
	uint16_t bytes_in_fifo= inertial_sensor_get_waiting_fifo_records_lenght();
	if(bytes_in_fifo>0)
	{
		volatile uint8_t accelerometer_temp_FIFO_buffer[bytes_in_fifo];
		inertial_sensor_read_FIFO((uint8_t *)accelerometer_temp_FIFO_buffer,bytes_in_fifo);
	}
}


void inertial_sensor_enable_fifo_stream(void) {
	inertial_sensor_set_register(ADXL362_REG_FIFO_CTL, FIFO_CONTROL_STREAM_AND_SAMPLE_HIGH_BIT);
	inertial_sensor_set_register(ADXL362_REG_FIFO_SAMPLES, 0x00); // We set 9.bit to 1 in fifo_control and these 8bits to 0. At the end we set 256 to watermark interrupt
	inertial_sensor_set_register(ADXL362_REG_INTMAP2,	INTMAP2_ONLY_FIFO_WATERMARK);
	//inertial_sensor_set_register(ADXL362_REG_FILTER_CTL, inertial_sensor_get_filter_controls(ODR_400, 0x01, RANGE_2G));
	//inertial_sensor_set_register(ADXL362_REG_POWER_CTL, inertial_sensor_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00,	ADXL_NOISE_MODE_ULTRALOW));
	inertial_sensor_clear_interrupt_bits();

}

void inertial_sensor_disable_fifo_stream(void)
{
	//inertial_sensor_set_register(ADXL362_REG_FIFO_CTL,0x08);
	inertial_sensor_set_register(ADXL362_REG_FIFO_CTL, FIFO_CONTROL_STREAM_AND_SAMPLE_HIGH_BIT);
	inertial_sensor_set_register(ADXL362_REG_INTMAP2,0x00);
	//inertial_sensor_set_register(ADXL362_REG_FILTER_CTL, inertial_sensor_get_filter_controls(ODR_400, 0x01, RANGE_2G));
	//inertial_sensor_set_register(ADXL362_REG_POWER_CTL, inertial_sensor_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_ULTRALOW));
	//inertial_sensor_clear_fifo();
	inertial_sensor_clear_interrupt_bits();
	GPIO_ClearITPendingBit(GPIO_Pin_5);
	GPIO_EXTICmd(GPIO_Pin_5, DISABLE);
}


void  inertial_sensor_sleep_and_enable_interrupt(void)
{
	//inertial_sensor_set_register(ADXL362_REG_FILTER_CTL, inertial_sensor_get_filter_controls(ODR_400, 0x01, RANGE_2G));
	//inertial_sensor_set_register(ADXL362_REG_POWER_CTL, inertial_sensor_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_ULTRALOW));
	inertial_sensor_set_register(ADXL362_REG_THRESH_ACT_L,state_manager_activity_threshold_low());
	inertial_sensor_set_register(ADXL362_REG_THRESH_ACT_H,0x00);
	inertial_sensor_set_register(ADXL362_REG_TIME_ACT,state_manager_activity_time());
	inertial_sensor_set_register(ADXL362_REG_ACT_INACT_CTL,0x03);
	inertial_sensor_set_register(ADXL362_REG_INTMAP1,INTMAP1_ONLY_ACTIVITY_INTERRUPT);
	inertial_sensor_get_register(ADXL362_REG_STATUS);

}




void inertial_sensor_read_FIFO(uint8_t* pBuffer,uint16_t length)
{
	uint8_t command[] = {ADXL362_SPI_COMMAND_FIFO_READ};
	sumer_spi_driver_read_data(SPI_DEVICE_ID_ADXL, pBuffer, command,1, length);
}



void inertial_sensor_disable_activity_interrupt(void)
{
	inertial_sensor_clear_interrupt_bits();
	inertial_sensor_set_register(ADXL362_REG_INTMAP1,0x00);

	/*APP_FLAG_CLEAR(WAITING_FOR_ACTIVITY);*/
}

void inertial_sensor_clear_interrupt_bits(void)
{
	inertial_sensor_get_register(ADXL362_REG_STATUS);
}



