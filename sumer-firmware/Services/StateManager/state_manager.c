#include "local_settings.h"
#include "flash_service.h"
#include "state_manager.h"

#define STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED 		0x01
#define STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT 	0x02
#define STATE_MANAGER_FIELD_STATUS_SYCNED 				0x03


uint8_t volatile is_seismic_log_enabled;
uint8_t volatile activity_threshold_low;
uint8_t volatile activity_time;
uint8_t volatile state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED;
uint8_t volatile state_of_activity_threshold_low=STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED;
uint8_t volatile state_of_activity_time=STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED;


void state_manager_check_and_set_defaults_to_flash(void){
//if flash is formatted state then set value defaults and commit them.
}

uint8_t state_manager_is_scribe_mode_enabled(void)
{
	return is_seismic_log_enabled;
}

uint8_t state_manager_activity_threshold_low(void)
{
	return activity_threshold_low;
}

uint8_t state_manager_activity_time(void)
{
	return activity_time;
}
void state_manager_set_is_scribe_mode_enabled(uint8_t new_value)
{
	is_seismic_log_enabled=new_value;
	state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT;
}

void state_manager_set_activity_threshold_low(uint8_t new_value)
{
	activity_threshold_low=new_value;
	state_of_activity_threshold_low=STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT;
}

void state_manager_set_activity_time(uint8_t new_value)
{
	activity_time=new_value;
	state_of_activity_time=STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT;
}


uint8_t * state_manager_local_name_val =NULL;
uint8_t * state_manager_server_local_name(void)
{

	if(state_manager_local_name_val==NULL)
	{
		state_manager_local_name_val=(uint8_t * )malloc(11 * sizeof(uint8_t));

		uint8_t * serial_number = state_manager_server_serial_number();
		char address_text_1[2];
		char address_text_2[2];
		char address_text_3[2];
		char address_text_4[2];
		sprintf(address_text_1, "%02X", serial_number[0]);
		sprintf(address_text_2, "%02X", serial_number[1]);
		sprintf(address_text_3, "%02X", serial_number[2]);
		sprintf(address_text_4, "%02X", serial_number[3]);
		state_manager_local_name_val[0]=0x09;
		state_manager_local_name_val[1]=(uint8_t)'S';
		state_manager_local_name_val[2]=(uint8_t)address_text_1[0];
		state_manager_local_name_val[3]=(uint8_t)address_text_1[1];
		state_manager_local_name_val[4]=(uint8_t)address_text_2[0];
		state_manager_local_name_val[5]=(uint8_t)address_text_2[1];
		state_manager_local_name_val[6]=(uint8_t)address_text_3[0];
		state_manager_local_name_val[7]=(uint8_t)address_text_3[1];
		state_manager_local_name_val[8]=(uint8_t)address_text_4[0];
		state_manager_local_name_val[9]=(uint8_t)address_text_4[1];
		state_manager_local_name_val[10]=(uint8_t)'R';
	}
	return state_manager_local_name_val;
}

uint8_t * serial_number=NULL;

uint8_t * state_manager_server_serial_number(void)
{
	if(serial_number==NULL)
	{
		serial_number = (uint8_t*)malloc(6 * sizeof(uint8_t));
		serial_number[0] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER);
		serial_number[1] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER+1);
		serial_number[2] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER+2);
		serial_number[3] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER+3);
		serial_number[4] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER+4);
		serial_number[5] = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER+5);
	}
	return serial_number;

}

void state_manager_initialize(void)
{
	state_manager_check_and_set_defaults_to_flash();
	is_seismic_log_enabled = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED);
	activity_threshold_low = local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_ACTIVITY_THRESHOLD_VAL);
	activity_time=	local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_ACTIVITY_TIME_VAL);
	state_of_is_sesimic_log_enabled = STATE_MANAGER_FIELD_STATUS_SYCNED;
	state_of_activity_threshold_low = STATE_MANAGER_FIELD_STATUS_SYCNED;
	state_of_activity_time = STATE_MANAGER_FIELD_STATUS_SYCNED;
}

void state_manager_commit_to_flash(void) {
	if (state_of_is_sesimic_log_enabled == STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT)
	{
		local_settings_set_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED,is_seismic_log_enabled);
		state_of_is_sesimic_log_enabled = STATE_MANAGER_FIELD_STATUS_SYCNED;
	}
	if (state_of_activity_threshold_low == STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT)
	{
		local_settings_set_char_value(STORAGE_FLASH_CHIP_ADDR_ACTIVITY_THRESHOLD_VAL,activity_threshold_low);
		state_of_activity_threshold_low = STATE_MANAGER_FIELD_STATUS_SYCNED;
	}
	if (state_of_activity_time == STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT)
	{
		local_settings_set_char_value(STORAGE_FLASH_CHIP_ADDR_ACTIVITY_TIME_VAL,activity_time);
		state_of_activity_time = STATE_MANAGER_FIELD_STATUS_SYCNED;
	}
}
