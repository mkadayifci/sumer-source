/**
 * @file    SDK_EVAL_SPI.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage SPI interface for eval board.
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_SERVICE_H
#define __SPI_SERVICE_H

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

#ifdef __cplusplus
 extern "C" {
#endif



/** @addtogroup SDK_EVAL_BlueNRG1       SDK EVAL BlueNRG1
 * @{
 */

/** @addtogroup SDK_EVAL_SPI            SDK EVAL SPI
 * @brief Management of SDK eval board SPI.
 * @{
 */

/** @defgroup SDK_EVAL_SPI_Exported_Types               SDK EVAL SPI Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_SPI_Exported_Constants                           SDK EVAL SPI Exported Constants
 * @{
 */

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

ErrorStatus spi_service_init(uint32_t baudrate);
ErrorStatus spi_service_read_data(uint8_t deviceId, uint8_t *pBuffer,uint8_t command[],uint8_t command_length,  uint16_t bytes_to_read);
ErrorStatus spi_service_write(uint8_t deviceId,uint8_t command[], uint16_t command_length);
ErrorStatus spi_service_write_data(uint8_t deviceId,uint8_t command[],uint16_t command_length,uint8_t *pBuffer, uint16_t bytes_to_write);

ErrorStatus SpiServiceRead(uint8_t deviceId,uint8_t* pBuffer, uint8_t RegisterAddr, uint8_t NumByteToRead);
ErrorStatus SpiServiceWrite(uint8_t deviceId,uint8_t RegisterAddr, uint8_t* pBuffer,  uint8_t NumByteToWrite);
ErrorStatus SpiServiceWriteSingle(uint8_t deviceId, uint8_t RegisterAddr,uint8_t value);


void ChangeSelectPin(uint8_t deviceId, uint8_t newState);

/**
 * @}
 */


/**
 * @}
 */



/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
