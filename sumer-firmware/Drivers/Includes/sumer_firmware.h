/*
 * sumer_state.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef SUMER_FIRMWARE_H_
#define SUMER_FIRMWARE_H_

#include "stdint.h"
#include "hal_types.h"

#define SUMER_FIRMWARE_WAIT_CYCLES_BEFORE_WINDOW_CHECK			0x1F4

#define SUMER_FIRMWARE_STATE_BLE_CONNECTED 						0x000001
#define SUMER_FIRMWARE_STATE_BLE_MAKE_CONNECTABLE 				0x000002
#define SUMER_FIRMWARE_STATE_BLE_NOTIFICATIONS_ENABLED	 		0x000004
#define SUMER_FIRMWARE_STATE_BLE_CONN_PARAM_UPD_SENT 			0x000008
#define SUMER_FIRMWARE_STATE_BLE_L2CAP_PARAM_UPD_SENT 			0x000010
#define SUMER_FIRMWARE_STATE_BLE_TX_BUFFER_FULL	 				0x000020
#define SUMER_FIRMWARE_STATE_BLE_SEND_DATA 						0x000040
#define SUMER_FIRMWARE_STATE_SCRIBE								0x000080
#define SUMER_FIRMWARE_STATE_BLE_START_READ_TX_CHAR_HANDLE		0x000100
#define SUMER_FIRMWARE_STATE_BLE_END_READ_TX_CHAR_HANDLE		0x000200
#define SUMER_FIRMWARE_STATE_BLE_START_READ_RX_CHAR_HANDLE		0x000400
#define SUMER_FIRMWARE_STATE_BLE_END_READ_RX_CHAR_HANDLE		0x000800
#define SUMER_FIRMWARE_STATE_SCRIBE_COOLDOWN					0x001000
#define SUMER_FIRMWARE_STATE_WAITING_FOR_ACTIVITY				0x002000
#define SUMER_FIRMWARE_STATE_IDLE								0x004000
#define SUMER_FIRMWARE_STATE_ACTIVITY_OCCURED					0x008000
#define SUMER_FIRMWARE_STATE_FIFO_OVERFLOW_OCCURED				0x010000


typedef struct{
	uint32_t firmware_state;
	uint8_t waiting_to_write_first_page;
} sumer_firmware_state_object_t;

volatile sumer_firmware_state_object_t * sumer_firmware_get_state_object(void);
void sumer_firmware_set_state_flag(uint32_t flag);
void sumer_firmware_clear_state_flag(uint32_t flag);
uint8_t sumer_firmware_check_state_flag(uint32_t flag);
void sumer_firmware_tick(void);
void sumer_firmware_init(void);
void sumer_firmware_set_activity_detection_mode(FunctionalState state);
static void sumer_firmware_do_cooldown_checks(void);
static void sumer_firmware_do_scribe_checks(void);
static void sumer_firmware_do_fifo_overflow_checks(void);
static void sumer_firmware_do_activity_occured_checks(void);
static uint8_t sumer_firmware_check_cycles_passed();

#endif /* SUMER_FIRMWARE_H_ */

