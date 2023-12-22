
/******************** (C) COPYRIGHT 2021 STMicroelectronics ********************
* File Name          : BLE_Beacon_main.c
* Author             : RF Application Team
* Version            : 1.1.0
* Date               : 15-January-2016
* Description        : Code demostrating the BLE Beacon application
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/*
 - (1): SIG company identifiers are available on https://www.bluetooth.org/en-us/specification/assigned-numbers/company-identifiers
 - NA : Not Applicable;
NOTEs:
     - OTA Service Manager support requires to build application by enabling only ST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 (preprocessor, linker) options and through files: OTA_btl.[ch] (refer to Release_with_OTA_ServiceManager IAR workspace).

**/
   
/** @addtogroup BlueNRG1_demonstrations_applications
 *  BlueNRG-1,2 Beacon demo \see BLE_Beacon_main.c for documentation.
 *
 *@{
 */

/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* Includes ------------------------------------------------------------------*/
#include "accelerometer.h"
#include <stdio.h>
#include <string.h>
#include <Sumer_config.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "sleep.h"
#include "OTA_btl.h"
#include "SPI_Service.h"
#include "serial_port.h"
#include "clock.h"
#include "sumer_clock.h"
#include "scribe.h"
#include "command_processor.h"
#include "OTA_btl.h"
#include "flash_service.h"
#include "local_settings.h"
#include "app_state.h"
#include "state_manager.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLE_BEACON_VERSION_STRING "1.1.0"

/* Set to 1 for enabling Flags AD Type position at the beginning 
   of the advertising packet */
#define ENABLE_FLAGS_AD_TYPE_AT_BEGINNING 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



void Radio_Init(){
	uint8_t ret;
	ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
		while (1);
	}

	ret = BluetoothDeviceInit();
	if (ret != BLE_STATUS_SUCCESS) {
		printf("SerialPort_DeviceInit()--> Failed 0x%02x\r\n", ret);
		while (1);
	}

}

/*******************************************************************************
* Function Name  : APP_Tick.
* Description    : Tick to run the application state machine.
* Input          : none.
* Return         : none.
*******************************************************************************/
void APP_Tick(void)
{

	if (APP_FLAG(SET_CONNECTABLE)) {
		Make_Connection();
		APP_FLAG_CLEAR(SET_CONNECTABLE);
	}

	flush_ble_serial_buffer();
	command_processor_parse_buffer();
	scribe_tick();
#if REQUEST_CONN_PARAM_UPDATE
  if(APP_FLAG(CONNECTED) && !APP_FLAG(L2CAP_PARAM_UPD_SENT) && Timer_Expired(&l2cap_req_timer))
  {
    aci_l2cap_connection_parameter_update_req(connection_handle, 8, 16, 0, 600);
    APP_FLAG_SET(L2CAP_PARAM_UPD_SENT);
  }
#endif


}/* end APP_Tick() */

void InitializeAllSystems(void){
	SystemInit();
	Clock_Init();
	Radio_Init();
	//SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_PKA, DISABLE);
	spi_service_init(SUMER_SPI_BAUDRATE);
	sumer_clock_init();
	accelerometer_init();
	storage_resume_deep_sleep_mode();
	if(state_manager_is_scribe_mode_enabled())
	{
		APP_FLAG_SET(WAITING_FOR_ACTIVITY);
		accelerometer_sleep_and_enable_interrupt();
	}
	storage_enter_deep_sleep_mode();
}

int main(void)
{
 	InitializeAllSystems();

 	while (1)
 	{
		APP_Tick();
 		BTLE_StackTick();
		BlueNRG_Sleep(SLEEPMODE_NOTIMER, WAKEUP_IO12, (WAKEUP_IOx_HIGH << WAKEUP_IO12_SHIFT_MASK) );
	}
}

/* Hardware Error event. 
   This event is used to notify the Host that a hardware failure has occurred in the Controller. 
   Hardware_Code Values:
   - 0x01: Radio state error
   - 0x02: Timer overrun error
   - 0x03: Internal queue overflow error
   After this event is recommended to force device reset. */

void hci_hardware_error_event(uint8_t Hardware_Code)
{
   NVIC_SystemReset();
}


/****************** BlueNRG-1,2 Sleep Management Callback ********************************/

SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
	return SLEEPMODE_RUNNING;

	if (APP_FLAG(SCRIBE_MODE)||APP_FLAG(CONNECTED) )
		return SLEEPMODE_RUNNING;
  
	return SLEEPMODE_NOTIMER;
}

/***************************************************************************************/

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
/** \endcond
 */
