/**
  ******************************************************************************
  * @file    BlueNRG1_it.c 
  * @author  VMA RF Application Team
  * @version V1.0.0
  * @date    September-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "interrupt_manager.h"
#include "inertial_sensor.h"
#include "sumer_firmware.h"
#include "clock.h"
/** @addtogroup BlueNRG1_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_Examples
  * @{
  */ 

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
  SysCount_Handler(); 
}

uint8_t io_interrupt_disabled_count=0;
void disable_io_interrupts(void)
{
	io_interrupt_disabled_count++;
	NVIC_DisableIRQ(GPIO_IRQn);
}

void enable_io_interrupts(void)
{
	if(io_interrupt_disabled_count==1){
		NVIC_EnableIRQ(GPIO_IRQn);
	}
	io_interrupt_disabled_count--;
}


void GPIO_Handler(void)
{
	disable_io_interrupts();

	if (interrupt_manager_get_interrupt_pin_value(INTERRUPT_MANAGER_ACTIVITY_PIN) == SET) //Activity Detected
	{
		interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_ACTIVITY_PIN,DISABLE);
		sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_ACTIVITY_OCCURED);
	}

	if (interrupt_manager_get_interrupt_pin_value(INTERRUPT_MANAGER_FIFO_WATERMARK_PIN) == SET)
	{
		interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_FIFO_WATERMARK_PIN,DISABLE);
		sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_FIFO_OVERFLOW_OCCURED);
	}

	enable_io_interrupts();
}

/******************************************************************************/
/*                 BlueNRG-1 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (system_BlueNRG1.c).                                               */
/******************************************************************************/
/**
* @brief  This function handles UART interrupt request.
* @param  None
* @retval None
*/
void UART_Handler(void)
{  
}

void Blue_Handler(void)
{
   // Call RAL_Isr
   RAL_Isr();
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

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
