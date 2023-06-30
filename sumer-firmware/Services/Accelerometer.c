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
	SetPowerControl();

}

void SetPowerControl(){
	POWER_CTRL ADXL_POWER_CTL = { 0 };
	ADXL_POWER_CTL.MEASURE = ADXL362_POWER_CTRL_STANDBY;
	ADXL_POWER_CTL.AUTOSLEEP = ADXL362_RESET;
	ADXL_POWER_CTL.WAKEUP = ADXL362_SET;
	ADXL_POWER_CTL.LOW_NOISE = ADXL362_NOISE_ULTRALOW;
	ADXL_POWER_CTL.EXT_CLK = ADXL362_RESET;
	//IvmeSpiWriteSingle(ADXL362_REG_POWER_CTL, (uint8_t)ADXL_POWER_CTL);


	Delay();
	SleepSensor();
	InitEXTI();
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
	uint8_t rBuffer;
	SpiServiceRead(SPI_DEVICE_ID_ADXL, &rBuffer, ADXL362_REG_STATUS, 1);

	//Sets ACT(4) bit HIGH to enable activity interrupt
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_INTMAP2, 0x10);

	//Activity Threshold Limit in mg
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_THRESH_ACT_L, 0x14);
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_THRESH_ACT_H, 0);

	//only referenced activity mode active
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_ACT_INACT_CTL, 0x03);
	//100Hz and Halved Bandwidth
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_FILTER_CTL, 0x13);
	// Low Noise, Wakeup, Standby
	SpiServiceWriteSingle(SPI_DEVICE_ID_ADXL, ADXL362_REG_POWER_CTL, 0x18);

}

void DoReset() {
	SpiServiceWriteSingle (SPI_DEVICE_ID_ADXL,ADXL362_REG_SOFT_RESET, ADXL362_SOFT_RESET_VAL);
	Delay();
}



