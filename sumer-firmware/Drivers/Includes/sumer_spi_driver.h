/*
 * sumer_spi_firmware.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef SUMER_SPI_DRIVER_H_
#define SUMER_SPI_DRIVER_H_

#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

/**
  * @brief SPI Baudrate
  */
#define SUMER_SPI_BAUDRATE                    (1000000)

/**
 * @brief  SPI DR address
 */
#define SPI_DR_BASE_ADDR                (SPI_BASE + 0x08)

/**
 * @brief  SPI buffer max size
 */
//#define SPI_BUFF_SIZE                   (2)

/**
 * @brief  SPI_TX DMA channel
 */
#define DMA_CH_SPI_TX                   (DMA_CH5)

/**
 * @brief  SPI_TX DMA Transfer Complete Interrupt
 */
#define DMA_CH_SPI_TX_IT_TC             (DMA_FLAG_TC5)

/**
 * @brief  SPI_RX DMA channel
 */
#define DMA_CH_SPI_RX                   (DMA_CH4)

/**
 * @brief  SPI_RX DMA Transfer Complete Interrupt
 */
#define DMA_CH_SPI_RX_IT_TC             (DMA_FLAG_TC4)

/**
 * @}
 */


#define SPI_DUMMY	            		 0x00


/**
 * @defgroup SDK_EVAL_SPI_Exported_Macros                       SDK EVAL SPI Exported Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup SDK_EVAL_SPI_Exported_Functions                   SDK EVAL SPI Exported Functions
 * @{
 */

#define SPI_DEVICE_ID_ADXL						1
#define SPI_DEVICE_ID_FLASH						2
#define SPI_DEVICE_ID_CLOCK						3

#define SPI_PERIPH_PIN_ADXL						GPIO_Pin_1
#define SPI_PERIPH_PIN_FLASH					GPIO_Pin_11
#define SPI_PERIPH_PIN_CLOCK					GPIO_Pin_8

#define SPI_ADXL_CS_HIGH()                  	GPIO_SetBits(SPI_PERIPH_PIN_ADXL)
#define SPI_ADXL_CS_LOW()                   	GPIO_ResetBits(SPI_PERIPH_PIN_ADXL)

#define SPI_FLASH_CS_HIGH()                  	GPIO_SetBits(SPI_PERIPH_PIN_FLASH)
#define SPI_FLASH_CS_LOW()                   	GPIO_ResetBits(SPI_PERIPH_PIN_FLASH)

#define SPI_CLOCK_CS_HIGH()                  	GPIO_SetBits(SPI_PERIPH_PIN_CLOCK)
#define SPI_CLOCK_CS_LOW()                   	GPIO_ResetBits(SPI_PERIPH_PIN_CLOCK)

ErrorStatus sumer_spi_driver_init(uint32_t baudrate);
ErrorStatus sumer_spi_driver_read_data(uint8_t deviceId, uint8_t *pBuffer,uint8_t command[],uint8_t command_length,  uint16_t bytes_to_read);
ErrorStatus sumer_spi_driver_write(uint8_t deviceId,uint8_t command[], uint16_t command_length);
ErrorStatus sumer_spi_driver_write_data(uint8_t deviceId,uint8_t command[],uint16_t command_length,uint8_t *pBuffer, uint16_t bytes_to_write);


void sumer_spi_driver_change_select_pin(uint8_t deviceId, uint8_t newState);


#endif /* SUMER_SPI_DRIVER_H_ */
