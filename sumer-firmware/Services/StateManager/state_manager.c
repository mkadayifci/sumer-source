#include "local_settings.h"
#include "flash_service.h"

#define STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED 		0x01
#define STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT 	0x02
#define STATE_MANAGER_FIELD_STATUS_SYCNED 				0x03


uint8_t volatile is_seismic_log_enabled;
uint8_t volatile state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_NOT_INITIALIZED;

uint8_t state_manager_is_scribe_mode_enabled(void)
{
	return is_seismic_log_enabled;
}
void state_manager_set_is_scribe_mode_enabled(uint8_t new_value)
{
	is_seismic_log_enabled=new_value;
	state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT;
}
void state_manager_initialize(void)
{
	is_seismic_log_enabled=	local_settings_get_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED);
	state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_SYCNED;
}
void state_manager_commit_to_flash()
{
	if(state_of_is_sesimic_log_enabled == STATE_MANAGER_FIELD_STATUS_WAITING_TO_COMMIT)
	{
		local_settings_set_char_value(STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED,is_seismic_log_enabled);
		state_of_is_sesimic_log_enabled=STATE_MANAGER_FIELD_STATUS_SYCNED;
	}
}
