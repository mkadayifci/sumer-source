#include "SPI_Service.h"
#include "Accelerometer.h"



void Delay(){
	for (uint32_t i = 0; i < 1000000; i++)__NOP();
}

void InitAccelerometer() {

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = ENABLE;

	GPIO_Init(&GPIO_InitStructure);

	DoReset();
	Delay();
	SetPowerControl();
	//InitEXTI();

}

void SetPowerControl(){


	SleepSensor();

}


void InitEXTI(){


	  /* Set the GPIO interrupt priority and enable it */
	  NVIC_InitType NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  GPIO_EXTIConfigType GPIO_EXTIStructure;
	  GPIO_EXTIStructure.GPIO_Pin =GPIO_Pin_5;
	  GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Level;
	  GPIO_EXTIStructure.GPIO_Event = GPIO_Event_High;
	  GPIO_EXTIConfig(&GPIO_EXTIStructure);


	  /* Clear pending interrupt */
	  GPIO_ClearITPendingBit(GPIO_Pin_5);

	  /* Enable the interrupt */
	  GPIO_EXTICmd(GPIO_Pin_5, ENABLE);


}

void SleepSensor(){

	//Sets ACT(4) bit HIGH to enable activity interrupt
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_INTMAP2, 0x10);

	//Activity Threshold Limit in mg
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_THRESH_ACT_L, 0x1E);
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_THRESH_ACT_H, 0x00);


	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_TIME_ACT, 1);



	//only referenced activity mode active
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_ACT_INACT_CTL, 0x03);
	//100Hz and Halved Bandwidth
	//SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_FILTER_CTL, 0x13);

	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_FILTER_CTL, 0x17);

	// Low Noise, Wakeup, Standby
	//SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_POWER_CTL, 0x18);
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_POWER_CTL, 0x2A);

	//SpiServiceRead(SPI_DEVICE_ID_ADXL, &rBuffer, ADXL362_REG_XDATA_L, 1);

}

void accelerometer_read_FIFO(uint8_t * pBuffer,uint8_t length){
	uint8_t command[] = {ADXL362_SPI_COMMAND_FIFO_READ};
	spi_service_read(SPI_DEVICE_ID_ADXL, pBuffer, command, length);
}

void DoReset() {
	SpiServiceWriteSingle (SPI_DEVICE_ID_ADXL,ADXL362_REG_SOFT_RESET, ADXL362_SOFT_RESET_VAL);
	Delay();
}



