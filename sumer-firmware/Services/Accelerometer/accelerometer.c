#include "SPI_Service.h"
#include "Accelerometer.h"

void delay(){
	for (uint32_t i = 0; i < 1000000; i++)__NOP();
}

void accelerometer_init() {


	accelerometer_reset();
	accelerometer_spi_write_single(ADXL362_REG_FIFO_CTL,0x0);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP2,0x0);
	accelerometer_sleep_sensor();


	InitEXTI();
}

void InitEXTI(){
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

void accelerometer_sleep_sensor(){
	//Sets ACT(4) bit HIGH to enable activity interrupt
	accelerometer_spi_write_single(ADXL362_REG_INTMAP1,0x10);
	//Activity Threshold Limit in mg
	accelerometer_spi_write_single(ADXL362_REG_THRESH_ACT_L,0x14);
	accelerometer_spi_write_single(ADXL362_REG_THRESH_ACT_H,0x00);
	accelerometer_spi_write_single(ADXL362_REG_TIME_ACT,0x01);
	//only referenced activity mode active
	accelerometer_spi_write_single(ADXL362_REG_ACT_INACT_CTL,0x03);
	//100Hz and Halved Bandwidth
	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL,0x17);
	// Low Noise, Wakeup, Standby
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL,0x2A);
	accelerometer_spi_read_single(ADXL362_REG_STATUS);

	GPIO_ClearITPendingBit(GPIO_Pin_12);
	GPIO_EXTICmd(GPIO_Pin_12, ENABLE);
}


void accelerometer_set_fifo_to_stream_mode(void){
	accelerometer_spi_write_single(ADXL362_REG_FIFO_CTL,0x2);
	accelerometer_spi_write_single(ADXL362_REG_FIFO_SAMPLES,0xFF);
	accelerometer_spi_write_single(ADXL362_REG_INTMAP2,0x4);
	accelerometer_spi_write_single(ADXL362_REG_FILTER_CTL,0x11);
	accelerometer_spi_write_single(ADXL362_REG_POWER_CTL,0x22);
	GPIO_ClearITPendingBit(GPIO_Pin_5);
	GPIO_EXTICmd(GPIO_Pin_5, ENABLE);
}


ErrorStatus accelerometer_spi_write_single(uint8_t command, uint8_t value) {

	return spi_service_write(
							SPI_DEVICE_ID_ADXL,

							(uint8_t[]) {
										ADXL362_SPI_COMMAND_WRITE,
										command,
										value },
							3);
}

uint8_t accelerometer_spi_read_single(uint8_t command) {
	uint8_t buffer[1];

	spi_service_read_data(
					SPI_DEVICE_ID_ADXL,
					(uint8_t*) &buffer,
					(uint8_t[] ) {
									ADXL362_SPI_COMMAND_READ,
									command },
					2,
					2);
	return buffer[0];
}

void accelerometer_read_FIFO(uint8_t * pBuffer,uint8_t length){
	uint8_t command[] = {ADXL362_SPI_COMMAND_FIFO_READ};
	spi_service_read_data(SPI_DEVICE_ID_ADXL, pBuffer, command,1, length);
}

void accelerometer_reset() {
	accelerometer_spi_write_single(ADXL362_REG_SOFT_RESET,ADXL362_SOFT_RESET_VAL);
	delay();
}
