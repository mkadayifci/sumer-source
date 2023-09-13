/* Includes ------------------------------------------------------------------*/
#include "SPI_Service.h"

#include "../../sumer-library/BlueNRG1_Periph_Driver/inc/BlueNRG1_spi.h"

/** @addtogroup SDK_EVAL_BlueNRG1         SDK EVAL BlueNRG1
 * @{
 */

/** @addtogroup SDK_EVAL_SPI              SDK EVAL SPI
 * @{
 */

/** @defgroup SDK_EVAL_SPI_Private_TypesDefinitions       SDK EVAL SPI Private Types Definitions
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_SPI_Private_Defines                SDK EVAL SPI Private Defines
 * @{
 */

/**
 * @brief  SPI IRQ mode defines
 */
#define SEND_START       0
#define SEND_READ_REG    1
#define SEND_WRITE_REG   2
#define SEND_WRITE_DATA  3

/**
 * @}
 */

/** @defgroup SDK_EVAL_SPI_Private_Variable               SDK EVAL SPI Private Variable
 * @{
 */

/**
 * @brief  SPI buffers used for DMA application
 */
volatile uint8_t spi_buffer_tx[10];
volatile uint8_t spi_buffer_rx[10];

/**
 * @brief  SPI flag set at end of transaction
 */
volatile FlagStatus spi_eot = SET;

/*static uint16_t byte_to_send = 0;
static uint8_t *spi_buffer_ptr;
static uint8_t buffer_idx = 0;
static uint8_t byte_to_send_max = 0;
static uint8_t register_addr = 0;
static uint8_t spi_irq_state = SEND_START;*/

/**
 * @}
 */

/** @defgroup SDK_EVAL_SPI_Private_Macros                 SDK EVAL SPI Private Macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_SPI_Private_FunctionPrototypes      SDK EVAL SPI Private Function Prototypes
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_SPI_Private_Functions               SDK EVAL SPI Private Functions
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup SDK_EVAL_SPI_Public_Functions                SDK EVAL SPI Public Functions
 * @{
 */

/**
 * @brief  Configures SPI interface
 * @param  baudrate: SPI clock frequency
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus spi_service_init(uint32_t baudrate) {
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
	GPIO_InitStructure.GPIO_Pin = SPI_PERIPH_PIN_ADXL | SPI_PERIPH_PIN_FLASH | SPI_PERIPH_PIN_CLOCK;
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

/**
 * @brief  SPI function to read registers from a slave device
 * @param  pBuffer: buffer to retrieve data from a slave
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to read
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus SpiServiceRead(uint8_t deviceId,uint8_t *pBuffer, uint8_t RegisterAddr,
		uint8_t NumByteToRead) {
	/* Set communication mode */
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);



	ChangeSelectPin(deviceId,RESET);

	/* Write data to send to TX FIFO */
	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));

	if (deviceId == SPI_DEVICE_ID_ADXL)
		SPI_SendData(0x0B);

	SPI_SendData(RegisterAddr);


	//SPI_ClearRXFIFO();

	for (uint8_t i = 0; i < NumByteToRead; i++) {
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(0x00);
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		pBuffer[i] = SPI_ReceiveData();
	}

	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));


	ChangeSelectPin(deviceId,SET);

	return SUCCESS;
}

/**
 * @brief  SPI function to write registers of a slave device
 * @param  pBuffer: buffer contains data to write
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to write
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus SpiServiceWrite(uint8_t deviceId, uint8_t RegisterAddr,uint8_t *pBuffer, uint8_t NumByteToWrite) {
	/* Set communication mode */


	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	ChangeSelectPin(deviceId,RESET);

	if (deviceId == SPI_DEVICE_ID_ADXL)
		SPI_SendData(0x0A);
	SPI_SendData(RegisterAddr);
	for (uint8_t i = 0; i < NumByteToWrite; i++)
		SPI_SendData(pBuffer[i]);

	/* Wait until data transfer is finished */
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));


	ChangeSelectPin(deviceId,SET);
	return SUCCESS;
}


ErrorStatus SpiServiceWriteSingle( uint8_t deviceId,uint8_t RegisterAddr,uint8_t value) {
	/* Set communication mode */


	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	ChangeSelectPin(deviceId,RESET);
	if (deviceId == SPI_DEVICE_ID_ADXL)
		SPI_SendData(0x0A);
	SPI_SendData(RegisterAddr);
	SPI_SendData(value);

	/* Wait until data transfer is finished */
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	ChangeSelectPin(deviceId,SET);
	return SUCCESS;
}


void ChangeSelectPin(uint8_t deviceId, uint8_t newState) {
	if (newState) {
		if (deviceId == SPI_DEVICE_ID_ADXL)
			GPIO_SetBits(SPI_PERIPH_PIN_ADXL);
		else if (deviceId == SPI_DEVICE_ID_FLASH)
			GPIO_SetBits(SPI_PERIPH_PIN_FLASH);
	} else {
		if (deviceId == SPI_DEVICE_ID_ADXL)
			GPIO_ResetBits(SPI_PERIPH_PIN_ADXL);
		else if (deviceId == SPI_DEVICE_ID_FLASH)
			GPIO_ResetBits(SPI_PERIPH_PIN_FLASH);
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
ErrorStatus spi_service_read_data(uint8_t deviceId, uint8_t *pBuffer,uint8_t command[],uint8_t command_length, uint8_t bytes_to_read) {


	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);

	ChangeSelectPin(deviceId, RESET);

	for (uint8_t i = 0; i < command_length; i++) {
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(command[i]);
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		SPI_ReceiveData();
	}

	for (uint8_t i = 0; i < bytes_to_read; i++) {
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(0x00);
		while (RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		pBuffer[i] = SPI_ReceiveData();
	}

	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	ChangeSelectPin(deviceId, SET);

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
ErrorStatus spi_service_write(uint8_t deviceId,uint8_t command[], uint16_t command_length){

	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	ChangeSelectPin(deviceId, RESET);

	for (uint16_t i = 0; i < command_length; i++) {
		SPI_SendData(command[i]);
	}
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));


	ChangeSelectPin(deviceId, SET);
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
ErrorStatus spi_service_write_data(uint8_t deviceId,uint8_t command[],uint16_t command_length,uint8_t *pBuffer, uint16_t bytes_to_write){

	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	ChangeSelectPin(deviceId, RESET);

	for (uint16_t i = 0; i < command_length; i++) {
		SPI_SendData(command[i]);
	}


	for (uint8_t i = 0; i < bytes_to_write; i++) {
		SPI_SendData(pBuffer[i]);
	}


	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));


	ChangeSelectPin(deviceId, SET);
	return SUCCESS;

}


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
