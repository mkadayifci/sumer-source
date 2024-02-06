/******************** (C) COPYRIGHT 2021 STMicroelectronics ********************
* File Name          : serial_port.c
* Author             : AMS - RF  Application team
* Version            : V1.1.0
* Date               : 24-August-2021
* Description        : This file handles bytes received from USB and the init
*                      function.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "gp_timer.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "bluenrg1_api.h"
#include "osal.h"
#include "gatt_db.h"
#include "serial_port.h"
#include "SDK_EVAL_Config.h"
#include "OTA_btl.h"
#include "command_processor.h"
#include "state_manager.h"
#include "sumer_firmware.h"
#include "flash_storage.h"


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

//#define SERVER_ADDRESS 0xAA, 0x34, 0x34, 0x08, 0xAB, 0x0D
//#define LOCAL_NAME  'S','3','4','0','8','A','B','0','D','R'
#define MANUF_DATA_SIZE (27)


#define CMD_BUFF_SIZE 512
static char cmd[CMD_BUFF_SIZE];
static uint16_t cmd_buff_end = 0, cmd_buff_start = 0;

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint8_t connInfo[20];




volatile uint16_t connection_handle = 0;

/**
  * @brief  Handle of TX,RX  Characteristics.
  */


/* UUIDs */
UUID_t UUID_Tx;
UUID_t UUID_Rx;




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SerialPort_DeviceInit.
* Description    : Init the Serial Port device.
* Input          : none.
* Return         : Status.
*******************************************************************************/
uint8_t BluetoothDeviceInit(void)
{
	uint8_t ret;
	uint16_t service_handle;
	uint16_t dev_name_char_handle;
	uint16_t appearance_char_handle;

	sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_MAKE_CONNECTABLE);

	uint8_t role = GAP_PERIPHERAL_ROLE;
	uint8_t * bdaddr = state_manager_server_serial_number();

	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,	CONFIG_DATA_PUBADDR_LEN,bdaddr);
	if (ret != BLE_STATUS_SUCCESS) {
		return ret;
	}

	aci_hal_set_tx_power_level(1, 7);

	ret = aci_gatt_init();
	if (ret != BLE_STATUS_SUCCESS) {
		return ret;
	} else {
	}

	ret = aci_gap_init(role, 0x00, 0x08, &service_handle, &dev_name_char_handle,
			&appearance_char_handle);
	if (ret != BLE_STATUS_SUCCESS) {
		return ret;
	} else {
	}

	/* Set the device name */
	/*ret = aci_gatt_update_char_value_ext(0, service_handle,
			dev_name_char_handle, 0, sizeof(name), 0, sizeof(name), name);
	if (ret != BLE_STATUS_SUCCESS) {
		debug_param(MESSAGE_LEVEL_ERROR, DEBUG_GATT_UPDATE_CHAR_ERROR, ret);

		return ret;
	} else {
		debug(MESSAGE_LEVEL_VERBOSE, DEBUG_GATT_UPDATE_CHAR_SUCCESS);
	}*/

	hci_le_set_scan_response_data(0,NULL);

  ret = Add_SerialPort_Service();
	if (ret != BLE_STATUS_SUCCESS) {
		//printf("Error in Add_SerialPort_Service 0x%02x\r\n", ret);
		return ret;
	} else {
		//printf("Add_SerialPort_Service() --> SUCCESS\r\n");
	}



  return BLE_STATUS_SUCCESS;
}


void flush_ble_serial_buffer(void)
{


	if (!sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_BLE_SEND_DATA) || sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL))
	{
		return;
	}


	while (cmd_buff_start < cmd_buff_end) {
		uint32_t len = MIN(20, cmd_buff_end - cmd_buff_start);
		tBleStatus ret;
		ret = aci_gatt_update_char_value_ext(
											connection_handle,
											SerialPortServHandle,
											TXCharHandle, 1, len, 0, len,
											(uint8_t*) cmd + cmd_buff_start);
		if (ret == BLE_STATUS_INSUFFICIENT_RESOURCES) {
			sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL);
			return;
		}
		else if  (ret == BLE_STATUS_SUCCESS) {
			cmd_buff_start += len;
		}
		else{
			sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL);
		}

	}



	// All data from buffer have been sent.
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_SEND_DATA);
	cmd_buff_end = 0;
	cmd_buff_start=0;
	//NVIC_EnableIRQ(UART_IRQn);

}

void start_advertising(void){

	uint16_t interval_in_ms= 10000;
	uint8_t * local_name = state_manager_server_local_name();
	aci_gap_set_discoverable(ADV_IND, interval_in_ms / 0.625, interval_in_ms / 0.625, PUBLIC_ADDR, NO_WHITE_LIST_USE,
	                                11, local_name, 0, NULL, 0, 0);
}

void send_data_over_ble_serial(uint8_t* data,uint8_t length ){


	//Burada buffer asilirsa bi sey yapmak lazim
	for(int i =0 ;i<length;i++){

		cmd[cmd_buff_end]=data[i];
		cmd_buff_end++;

	}

	sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_SEND_DATA);

}
void send_data_over_ble_serial_and_force(uint8_t* data,uint8_t length ){

	send_data_over_ble_serial(data,length);
	flush_ble_serial_buffer();
	while (sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL))
	{
		BTLE_StackTick();
	}

}



/*******************************************************************************
* Function Name  : Make_Connection.
* Description    : If the device is a Client create the connection. Otherwise puts
*                  the device in discoverable mode.
* Input          : none.
* Return         : none.
*******************************************************************************/
void Make_Connection(void)
{
  tBleStatus ret;


#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  hci_le_set_scan_response_data(18,BTLServiceUUID4Scan);
#else
  hci_le_set_scan_response_data(0,NULL);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */

  start_advertising();

  if(ret != BLE_STATUS_SUCCESS)
  {
    //printf ("Error in aci_gap_set_discoverable(): 0x%02x\r\n", ret);
  }
  else
  {
    //printf ("aci_gap_set_discoverable() --> SUCCESS\r\n");
  }

  /* Update Advertising data with manuf_data */
  //aci_gap_update_adv_data(MANUF_DATA_SIZE, manuf_data);


}




/* ***************** BlueNRG-1 Stack Callbacks ********************************/

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)

{
  connection_handle = Connection_Handle;

  sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_CONNECTED);
  sumer_firmware_set_activity_detection_mode(DISABLE);
  //scribe_stop_without_cooldown(); TODO:Check
  flash_storage_exit_deep_sleep_mode();

#if REQUEST_CONN_PARAM_UPDATE
  APP_FLAG_CLEAR(L2CAP_PARAM_UPD_SENT);
  Timer_Set(&l2cap_req_timer, CLOCK_SECOND*2);
#endif
}/* end hci_le_connection_complete_event() */

/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_CONNECTED);
	sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_MAKE_CONNECTABLE);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_NOTIFICATIONS_ENABLED);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL);

	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_START_READ_TX_CHAR_HANDLE);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_END_READ_TX_CHAR_HANDLE);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_START_READ_RX_CHAR_HANDLE);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_END_READ_RX_CHAR_HANDLE);





	state_manager_commit_to_flash();
	if (state_manager_is_scribe_mode_enabled() == 1) {
		sumer_firmware_set_activity_detection_mode(ENABLE);
	}
	flash_storage_enter_deep_sleep_mode();
	start_advertising();



#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  OTA_terminate_connection();
#endif

}/* end hci_disconnection_complete_event() */


/*******************************************************************************
 * Function Name  : aci_gatt_attribute_modified_event.
 * Description    : This event occurs when an attribute is modified.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{
#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  OTA_Write_Request_CB(Connection_Handle, Attr_Handle, Attr_Data_Length, Attr_Data);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */

  Attribute_Modified_CB(Attr_Handle, Attr_Data_Length, Attr_Data);
}



/*******************************************************************************
 * Function Name  : aci_gatt_tx_pool_available_event.
 * Description    : This event occurs when a TX pool available is received.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers)
{
  /* It allows to notify when at least 2 GATT TX buffers are available */
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL);
}

/*******************************************************************************
 * Function Name  : aci_gatt_read_permit_req_event.
 * Description    : This event is given when a read request is received
 *                  by the server from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                    uint16_t Attribute_Handle,
                                    uint16_t Offset)
{
#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  /* Lower/Higher Applications with OTA Service */
  aci_gatt_allow_read(Connection_Handle);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */
}

void aci_hal_end_of_radio_activity_event(uint8_t Last_State,
                                         uint8_t Next_State,
                                         uint32_t Next_State_SysTime)
{
#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  if (Next_State == 0x02) /* 0x02: Connection event slave */
  {
    OTA_Radio_Activity(Next_State_SysTime);
  }
#endif
}
