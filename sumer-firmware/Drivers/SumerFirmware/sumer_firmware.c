/*
 * sumer_state.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#include <sumer_firmware.h>
#include "interrupt_manager.h"
#include "command_processor.h"
#include "flash_storage.h"
#include "inertial_sensor.h"
#include "sumer_scriber.h"
#include "state_manager.h"
#include "serial_port.h"

static volatile sumer_firmware_state_object_t sumer_state_object={0};


sumer_firmware_state_object_t * sumer_firmware_get_state_object(void)
{
	return &sumer_state_object;
}


void sumer_firmware_set_state_flag(uint32_t flag)
{
	sumer_state_object.firmware_state |= flag;
}

void sumer_firmware_clear_state_flag(uint32_t flag)
{
	sumer_state_object.firmware_state  &= ~flag;
}

uint8_t sumer_firmware_check_state_flag(uint32_t flag)
{
	return (sumer_state_object.firmware_state & flag)>0;
}

void sumer_firmware_init(void)
{

	interrupt_manager_init_mcu_external_interrupts();
}

void sumer_firmware_set_activity_detection_mode(FunctionalState state)
{
	if(state==ENABLE)
	{
		interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_ACTIVITY_PIN,ENABLE);
		inertial_sensor_sleep_and_enable_interrupt();
		sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_WAITING_FOR_ACTIVITY);
	}
	else if(state==DISABLE)
	{
		interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_ACTIVITY_PIN,DISABLE);
		inertial_sensor_disable_activity_interrupt();
		sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_WAITING_FOR_ACTIVITY);
	}
}

void sumer_firmware_tick(void)
{
	sumer_firmware_do_activity_occured_checks();
	sumer_firmware_do_fifo_overflow_checks();
	sumer_firmware_do_cooldown_checks();
	sumer_firmware_do_scribe_checks();

	flush_ble_serial_buffer();
	command_processor_parse_buffer();
}

static void sumer_firmware_do_activity_occured_checks(void)
{
	if(sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_ACTIVITY_OCCURED))
	{
		sumer_firmware_set_activity_detection_mode(DISABLE);
		inertial_sensor_disable_activity_interrupt();
		inertial_sensor_clear_interrupt_bits();
		sumer_scriber_start();
		flash_storage_exit_deep_sleep_mode();
		sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_ACTIVITY_OCCURED);
	}
}

static void sumer_firmware_do_fifo_overflow_checks(void)
{
	if(sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_FIFO_OVERFLOW_OCCURED))
	{
		sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_FIFO_OVERFLOW_OCCURED);
		sumer_scriber_write_log_page_from_inertial_sensor_fifo();
		if(!sumer_scriber_is_log_window_over()){
			interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_FIFO_WATERMARK_PIN,ENABLE);
			inertial_sensor_enable_fifo_stream();
		}
	}
}

static void sumer_firmware_do_scribe_checks(void)
{
	if(sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_SCRIBE))
	{
		if(sumer_scriber_is_log_window_over())
		{
			sumer_scriber_stop();
			flash_storage_enter_deep_sleep_mode();
		}
	}
}

static void sumer_firmware_do_cooldown_checks(void)
{
	if(sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_SCRIBE_COOLDOWN))
	{
		if(!sumer_scriber_is_in_cooldown_period())
		{
			sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_SCRIBE_COOLDOWN);
			if(state_manager_is_scribe_mode_enabled()==1)
			{
				if(!sumer_firmware_check_state_flag(SUMER_FIRMWARE_STATE_BLE_CONNECTED))
				{
					sumer_firmware_set_activity_detection_mode(ENABLE);
				}
			}
		}
	}
}
