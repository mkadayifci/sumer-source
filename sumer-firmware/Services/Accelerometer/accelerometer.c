#include "SPI_Service.h"
#include "Accelerometer.h"




void accelerometer_delay()
{
	for (uint32_t i = 0; i < 80000; i++)__NOP();
}

void accelerometer_init()
{
	accelerometer_reset();
	accelerometer_clear_fifo_stream_mode();
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL, accelerometer_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_LOW));
	//accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL, accelerometer_get_filter_controls(ODR_200, 0x00, RANGE_2G));
	accelerometer_spi_write_single(ADXL362_REG_FIFO_CTL, 0x00);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP2, 0x00);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP1, 0x00);
	accelerometer_init_external_interrupts();
}


uint8_t accelerometer_get_filter_controls(enum ADXL_ODR_VALUES odr,uint8_t half_bw,enum ADXL_RANGES range)
{

	adxl_filter_control_t filter_control={0};
	filter_control.ODR = odr;
	filter_control.EXT_SAMPLE= 0x00;
	filter_control.HALF_BW = half_bw;
	filter_control.RANGE = range;
	return  ((uint8_t *)&filter_control)[0];

}

uint8_t accelerometer_get_power_controls(enum ADXL_MODES mode,uint8_t autosleep,uint8_t wakeup,enum ADXL_NOISE_MODES noise_mode)
{

	adxl_power_control_t power_control={0};
	power_control.MEASURE= mode;
	power_control.AUTOSLEEP=autosleep;
	power_control.WAKEUP= wakeup;
	power_control.LOW_NOISE=noise_mode;
	power_control.EXT_CLK=0x00;
	return ((uint8_t *)&power_control)[0];

}



void accelerometer_init_external_interrupts()
{
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = ENABLE;
	GPIO_Init(&GPIO_InitStructure);
	/* Set the GPIO interrupt priority and enable it */
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	GPIO_EXTIConfigType GPIO_EXTIStructure;
	GPIO_EXTIStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_12;
	GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Level;
	GPIO_EXTIStructure.GPIO_Event = GPIO_Event_High;
	GPIO_EXTIConfig(&GPIO_EXTIStructure);
}

void accelerometer_disable_activity_interrupt(void)
{
	accelerometer_spi_write_single(ADXL362_REG_INTMAP1,0x00);
}

void accelerometer_clear_interrupt_bits(void)
{
	accelerometer_spi_read_single(ADXL362_REG_STATUS);
}

void accelerometer_sleep_and_enable_interrupt()
{

	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL, accelerometer_get_filter_controls(ODR_50, 0x01, RANGE_2G));
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL, accelerometer_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_LOW));
	accelerometer_spi_write_single(ADXL362_REG_THRESH_ACT_L,0x10);
	accelerometer_spi_write_single(ADXL362_REG_THRESH_ACT_H,0x00);
	accelerometer_spi_write_single(ADXL362_REG_TIME_ACT,0x04);
	accelerometer_spi_write_single(ADXL362_REG_ACT_INACT_CTL,0x03);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP1,INTMAP1_ONLY_ACTIVITY_INTERRUPT);
	accelerometer_spi_read_single(ADXL362_REG_STATUS);
	GPIO_ClearITPendingBit(GPIO_Pin_12);
	GPIO_EXTICmd(GPIO_Pin_12, ENABLE);
}

void accelerometer_set_fifo_to_stream_mode(void) {
	accelerometer_spi_write_single(ADXL362_REG_FIFO_CTL, FIFO_CONTROL_STREAM_AND_SAMPLE_HIGH_BIT);
	accelerometer_spi_write_single(ADXL362_REG_FIFO_SAMPLES, 0x00); // We set 9.bit to 1 in fifo_control and these 8bits to 0. At the end we set 256 to watermark interrupt
	accelerometer_spi_write_single(ADXL362_REG_INTMAP2,	INTMAP2_ONLY_FIFO_WATERMARK);
	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL, accelerometer_get_filter_controls(ODR_400, 0x01, RANGE_2G));
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL, accelerometer_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00,	ADXL_NOISE_MODE_ULTRALOW));
	accelerometer_clear_interrupt_bits();
	GPIO_ClearITPendingBit(GPIO_Pin_5);
	GPIO_EXTICmd(GPIO_Pin_5, ENABLE);
}

void accelerometer_clear_fifo_stream_mode(void)
{
	accelerometer_spi_write_single(ADXL362_REG_FIFO_CTL,0x08);
	//accelerometer_spi_write_single(ADXL362_REG_FIFO_SAMPLES,0xFF);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP2,0x00);
	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL, accelerometer_get_filter_controls(ODR_25, 0x01, RANGE_2G));
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL, accelerometer_get_power_controls(ADXL_MODE_MEASURE, 0x00, 0x00, ADXL_NOISE_MODE_LOW));
	accelerometer_clear_interrupt_bits();
	GPIO_ClearITPendingBit(GPIO_Pin_5);
	GPIO_EXTICmd(GPIO_Pin_5, DISABLE);
}

ErrorStatus accelerometer_spi_write_single(uint8_t command, uint8_t value)
{
	return spi_service_write(
							SPI_DEVICE_ID_ADXL,

							(uint8_t[]) {
										ADXL362_SPI_COMMAND_WRITE,
										command,
										value },
							3);
}

uint8_t accelerometer_spi_read_single(uint8_t command)
{
	uint8_t buffer[1];
	spi_service_read_data(
					SPI_DEVICE_ID_ADXL,
					(uint8_t*) &buffer,
					(uint8_t[] ) {
									ADXL362_SPI_COMMAND_READ,
									command },
					2,
					1);
	return buffer[0];
}

void accelerometer_read_FIFO(uint8_t* pBuffer,uint16_t length)
{
	uint8_t command[] = {ADXL362_SPI_COMMAND_FIFO_READ};
	spi_service_read_data(SPI_DEVICE_ID_ADXL, pBuffer, command,1, length);
}

void accelerometer_reset()
{
	accelerometer_spi_write_single(ADXL362_REG_SOFT_RESET,ADXL362_SOFT_RESET_VAL);
	accelerometer_delay();
}
