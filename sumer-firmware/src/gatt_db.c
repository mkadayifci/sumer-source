#include <command_processor.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "osal.h"
#include "SDK_EVAL_Config.h"
#include "serial_port.h"
#include "sumer_firmware.h"

uint16_t SerialPortServHandle, TXCharHandle, RXCharHandle;

/* UUIDs */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;

/*******************************************************************************
* Function Name  : Add_SerialPort_Service
* Description    : Add the Serial Port service.
* Input          : None
* Return         : Status.
*******************************************************************************/
uint8_t Add_SerialPort_Service(void)
{
  uint8_t ret;

  /*
  UUIDs:

		6E400001-B5A3-F393-E0A9-E50E24DCCA9E
		6E400003-B5A3-F393-E0A9-E50E24DCCA9E
		6E400002-B5A3-F393-E0A9-E50E24DCCA9E

  */
  const uint8_t uuid[16] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e};
  const uint8_t charUuidRX[16] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e};
  const uint8_t charUuidTX[16] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e};

	Osal_MemCpy(&service_uuid.Service_UUID_128, uuid, 16);

	ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE, 6,
			&SerialPortServHandle);
	if (ret != BLE_STATUS_SUCCESS)
		goto fail;

	Osal_MemCpy(&char_uuid.Char_UUID_128, charUuidTX, 16);
	ret = aci_gatt_add_char(SerialPortServHandle, UUID_TYPE_128, &char_uuid, 20,
			CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, 0, 16, 1, &TXCharHandle);
	if (ret != BLE_STATUS_SUCCESS)
		goto fail;

	Osal_MemCpy(&char_uuid.Char_UUID_128, charUuidRX, 16);

	ret = aci_gatt_add_char(SerialPortServHandle,
	UUID_TYPE_128, &char_uuid, 20,
	CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP,
	ATTR_PERMISSION_NONE,
	GATT_NOTIFY_ATTRIBUTE_WRITE, 16, 1, &RXCharHandle);

	if (ret != BLE_STATUS_SUCCESS)
		goto fail;

  //printf("Serial Port Service added.\nTX Char Handle %04X, RX Char Handle %04X\n", TXCharHandle, RXCharHandle);
  return BLE_STATUS_SUCCESS;

fail:
  printf("Error while adding Serial Port service.\n");
  return BLE_STATUS_ERROR ;
}


/*******************************************************************************
* Function Name  : Attribute_Modified_CB
* Description    : Attribute modified callback.
* Input          : Attribute handle modified.
*                  Length of the data.
*                  Attribute data.
* Return         : None.
*******************************************************************************/
void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *att_data)
{

	if (handle == RXCharHandle + 1) {
		command_processor_add_to_buffer(att_data, data_length);
	} else if (handle == TXCharHandle + 2) {

		if (att_data[0] == 0x01)
		{
			sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_BLE_NOTIFICATIONS_ENABLED);
		}

	}

}


