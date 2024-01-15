/*
 * sumer_spi_firmware.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */
#include "sumer_spi_driver.h"
#include "BlueNRG1_it.h"
#include "../../sumer-library/BlueNRG1_Periph_Driver/inc/BlueNRG1_spi.h"



ErrorStatus sumer_spi_driver_init(uint32_t baudrate) {
	SPI_InitType SPI_InitStructure;
	GPIO_InitType GPIO_InitStructure;

	/* Enable SPI and GPIO clocks */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);

	/* Configure SPI pins */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;
	GPIO_InitStructure.GPIO_Pull = ENABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);



	/* Configure CS pins */
	GPIO_InitStructure.GPIO_Pin = SPI_PERIPH_PIN_ADXL | SPI_PERIPH_PIN_FLASH | SPI_PERIPH_PIN_CLOCK| GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Output;
	GPIO_Init(&GPIO_InitStructure);


	SPI_ADXL_CS_HIGH();
	SPI_FLASH_CS_HIGH();
	SPI_CLOCK_CS_HIGH();

	/* Configure SPI in master mode */
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_BaudRate = baudrate;

	SPI_Init(&SPI_InitStructure);


	/* Set null character */
	SPI_SetDummyCharacter(0x00);

	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);

	/* Clear RX and TX FIFO */
	SPI_ClearTXFIFO();
	SPI_ClearRXFIFO();



	/* Enable SPI functionality */
	SPI_Cmd(ENABLE);

	return SUCCESS;

}

void sumer_spi_driver_change_select_pin(uint8_t deviceId, uint8_t newState) {
	if (newState) {
		if (deviceId == SPI_DEVICE_ID_ADXL)
			GPIO_SetBits(SPI_PERIPH_PIN_ADXL);
		else if (deviceId == SPI_DEVICE_ID_FLASH)
			GPIO_SetBits(SPI_PERIPH_PIN_FLASH);
		else if (deviceId == SPI_DEVICE_ID_CLOCK)
			GPIO_SetBits(SPI_PERIPH_PIN_CLOCK);
	} else {
		if (deviceId == SPI_DEVICE_ID_ADXL)
			GPIO_ResetBits(SPI_PERIPH_PIN_ADXL);
		else if (deviceId == SPI_DEVICE_ID_FLASH)
			GPIO_ResetBits(SPI_PERIPH_PIN_FLASH);
		else if (deviceId == SPI_DEVICE_ID_CLOCK)
			GPIO_ResetBits(SPI_PERIPH_PIN_CLOCK);
	}
}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */




/**
 * @brief  SPI function to read registers from a slave device
 * @param  deviceId: application specific device to determine chip select
 * @param  pBuffer: buffer to retrieve data from a slave
 * @param  command: command for read
 * @param  bytes_to_read: number of byte to read
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus sumer_spi_driver_read_data(uint8_t deviceId, uint8_t *pBuffer,uint8_t command[],uint8_t command_length, uint16_t bytes_to_read) {


	disable_io_interrupts();


	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);


	sumer_spi_driver_change_select_pin(deviceId, RESET);

	for (uint8_t i = 0; i < command_length; i++) {
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(command[i]);
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		SPI_ReceiveData();
	}

	for (uint16_t i = 0; i < bytes_to_read; i++) {
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(0x00);
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		pBuffer[i] = SPI_ReceiveData();
	}

	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	sumer_spi_driver_change_select_pin(deviceId, SET);

	enable_io_interrupts();

	return SUCCESS;
}





/**
 * @brief  SPI function to read registers from a slave device
 * @param  deviceId: application specific device to determine chip select
 * @param  pBuffer: buffer to retrieve data from a slave
 * @param  command: command for read
 * @param  bytes_to_write: number of byte to write
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus sumer_spi_driver_write(uint8_t deviceId,uint8_t command[], uint16_t command_length){

	disable_io_interrupts();
	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	sumer_spi_driver_change_select_pin(deviceId, RESET);

	for (uint16_t i = 0; i < command_length; i++) {
		SPI_SendData(command[i]);
		while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
	}


	sumer_spi_driver_change_select_pin(deviceId, SET);

	enable_io_interrupts();
	return SUCCESS;

}

/**
 * @brief  SPI function to read registers from a slave device
 * @param  deviceId: application specific device to determine chip select
 * @param  pBuffer: buffer to retrieve data from a slave
 * @param  command: command for read
 * @param  bytes_to_write: number of byte to write
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus sumer_spi_driver_write_data(uint8_t deviceId,uint8_t command[],uint16_t command_length,uint8_t *pBuffer, uint16_t bytes_to_write){

	disable_io_interrupts();
	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	sumer_spi_driver_change_select_pin(deviceId, RESET);

	for (uint16_t i = 0; i < command_length; i++) {
		SPI_SendData(command[i]);
		while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
	}


	for (uint16_t i = 0; i < bytes_to_write; i++) {
		SPI_SendData(pBuffer[i]);
		while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
	}





	sumer_spi_driver_change_select_pin(deviceId, SET);
	enable_io_interrupts();
	return SUCCESS;

}

