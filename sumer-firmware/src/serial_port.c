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

#include <debug_functions.h>
#include <stdio.h>
#include <string.h>
#include "gp_timer.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "bluenrg1_api.h"
#include "app_state.h"
#include "osal.h"
#include "gatt_db.h"
#include "serial_port.h"
#include "SDK_EVAL_Config.h"
#include "OTA_btl.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/


#define SERVER_ADDRESS 0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02
#define LOCAL_NAME  'S','u','m','e','r','A','8','2','E'
#define MANUF_DATA_SIZE (27)


#define CMD_BUFF_SIZE 512
static char cmd[CMD_BUFF_SIZE];
static uint16_t cmd_buff_end = 0, cmd_buff_start = 0;

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint8_t connInfo[20];
volatile int app_flags = SET_CONNECTABLE;
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


	uint8_t role = GAP_PERIPHERAL_ROLE;
	uint8_t bdaddr[] = { 0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02 };

	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
	CONFIG_DATA_PUBADDR_LEN, bdaddr);
	if (ret != BLE_STATUS_SUCCESS) {
		debug_param(MESSAGE_LEVEL_ERROR,
				DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY,
				DEBUG_SETTING_BDADDR_ERROR, ret);
		return ret;
	}

	aci_hal_set_tx_power_level(1, 4);

	ret = aci_gatt_init();
	if (ret != BLE_STATUS_SUCCESS) {
		debug_param(MESSAGE_LEVEL_ERROR,
				DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY,
				DEBUG_GATT_INIT_ERROR, ret);
		return ret;
	} else {
		debug(MESSAGE_LEVEL_VERBOSE,
				DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY,
				DEBUG_GATT_INIT_SUCCESS);
	}

	ret = aci_gap_init(role, 0x00, 0x08, &service_handle, &dev_name_char_handle,
			&appearance_char_handle);
	if (ret != BLE_STATUS_SUCCESS) {
		debug_param(MESSAGE_LEVEL_ERROR,
				DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY,
				DEBUG_GAP_INIT_ERROR, ret);
		return ret;
	} else {
		debug(MESSAGE_LEVEL_VERBOSE,
				DEBUG_CONFIGURATION_INITIALIZATION_CATEGORY,
				DEBUG_GAP_INIT_SUCCESS);
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



  ret = Add_SerialPort_Service();
	if (ret != BLE_STATUS_SUCCESS) {
		printf("Error in Add_SerialPort_Service 0x%02x\r\n", ret);
		return ret;
	} else {
		//printf("Add_SerialPort_Service() --> SUCCESS\r\n");
	}

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  ret = OTA_Add_Btl_Service();
  if(ret == BLE_STATUS_SUCCESS)
    printf("OTA service added successfully.\n");
  else
    printf("Error while adding OTA service.\n");
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */



  return BLE_STATUS_SUCCESS;
}


void flush_ble_serial_buffer(void)
{

	if (!APP_FLAG(SEND_DATA) || APP_FLAG(TX_BUFFER_FULL))
		return;

	while (cmd_buff_start < cmd_buff_end) {
		uint32_t len = MIN(20, cmd_buff_end - cmd_buff_start);
		tBleStatus ret;
		ret = aci_gatt_update_char_value_ext(
											connection_handle,
											SerialPortServHandle,
											TXCharHandle, 1, len, 0, len,
											(uint8_t*) cmd + cmd_buff_start);
		if (ret == BLE_STATUS_INSUFFICIENT_RESOURCES) {
			APP_FLAG_SET(TX_BUFFER_FULL);
			return;
		}
		else if  (ret == BLE_STATUS_SUCCESS) {
			cmd_buff_start += len;
		}
		else{
			APP_FLAG_SET(TX_BUFFER_FULL);
		}

	}



	// All data from buffer have been sent.
	APP_FLAG_CLEAR(SEND_DATA);
	cmd_buff_end = 0;
	cmd_buff_start=0;
	//NVIC_EnableIRQ(UART_IRQn);

}

void send_data_over_ble_serial(uint8_t* data,uint8_t length ){


	//Burada buffer asilirsa bi sey yapmak lazim
	for(int i =0 ;i<length;i++){

		cmd[cmd_buff_end]=data[i];
		cmd_buff_end++;

	}

	APP_FLAG_SET(SEND_DATA);
}


/*******************************************************************************
* Function Name  : Process_InputData.
* Description    : Process a command. It should be called when data are received.
* Input          : data_buffer: data address.
*	           Nb_bytes: number of received bytes.
* Return         : none.
*******************************************************************************/
void Process_InputData(uint8_t* data_buffer, uint16_t Nb_bytes)
{
	return;
	/*
	uint8_t i;

	for (i = 0; i < Nb_bytes; i++) {
		if (cmd_buff_end >= CMD_BUFF_SIZE - 1) {
			cmd_buff_end = 0;
		}

		cmd[cmd_buff_end] = data_buffer[i];
		//SdkEvalComIOSendData(data_buffer[i]);
		cmd_buff_end++;

		if ((cmd[cmd_buff_end - 1] == '\n')
				|| (cmd[cmd_buff_end - 1] == '\r')) {
			if (cmd_buff_end != 1) {

				cmd[cmd_buff_end] = '\0'; // Only a termination character. Not strictly needed.

				// Set flag to send data. Disable UART IRQ to avoid overwriting buffer with new incoming data
				APP_FLAG_SET(SEND_DATA);
				NVIC_DisableIRQ(UART_IRQn);

				cmd_buff_start = 0;

			} else {
				cmd_buff_end = 0; // Discard
			}
		}
	}*/
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



  /* NOTE: Updated original Server advertising data in order to be also recognized by ìBLE Sensorî app Client */

  tBDAddr bdaddr = {SERVER_ADDRESS};

  uint8_t manuf_data[MANUF_DATA_SIZE] = {
    2,                      /* Length of AD type Transmission Power */
    0x0A, 0x00,             /* Transmission Power = 0 dBm */
    9,                      /* Length of AD type Complete Local Name */
    0x09,                   /* AD type Complete Local Name */
    LOCAL_NAME,             /* Local Name */
    13,                     /* Length of AD type Manufacturer info */
    0xFF,                   /* AD type Manufacturer info */
    0x01,                   /* Protocol version */
    0x05,		            /* Device ID: 0x05 */
    0x00,                   /* Feature Mask byte#1 */
    0x00,                   /* Feature Mask byte#2 */
    0x00,                   /* Feature Mask byte#3 */
    0x00,                   /* Feature Mask byte#4 */
    0x00,                   /* BLE MAC start */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00                    /* BLE MAC stop */
  };


  for (int var = 0; var < 6; ++var) {
    manuf_data[MANUF_DATA_SIZE -1  - var] = bdaddr[var];
  }

  uint8_t local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, LOCAL_NAME };

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  hci_le_set_scan_response_data(18,BTLServiceUUID4Scan);
#else
  hci_le_set_scan_response_data(0,NULL);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */

  ret = aci_gap_set_discoverable(ADV_IND, 0x90, 0x90, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
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

#if REQUEST_CONN_PARAM_UPDATE
  if(APP_FLAG(CONNECTED) && !APP_FLAG(L2CAP_PARAM_UPD_SENT) && Timer_Expired(&l2cap_req_timer))
  {
    aci_l2cap_connection_parameter_update_req(connection_handle, 8, 16, 0, 600);
    APP_FLAG_SET(L2CAP_PARAM_UPD_SENT);
  }
#endif


}/* end APP_Tick() */


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

  APP_FLAG_SET(CONNECTED);

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
   APP_FLAG_CLEAR(CONNECTED);
  /* Make the device connectable again. */
  APP_FLAG_SET(SET_CONNECTABLE);
  APP_FLAG_CLEAR(NOTIFICATIONS_ENABLED);
  APP_FLAG_CLEAR(TX_BUFFER_FULL);

  APP_FLAG_CLEAR(START_READ_TX_CHAR_HANDLE);
  APP_FLAG_CLEAR(END_READ_TX_CHAR_HANDLE);
  APP_FLAG_CLEAR(START_READ_RX_CHAR_HANDLE);
  APP_FLAG_CLEAR(END_READ_RX_CHAR_HANDLE);

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
  APP_FLAG_CLEAR(TX_BUFFER_FULL);
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
