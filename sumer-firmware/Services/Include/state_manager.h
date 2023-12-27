/*
 * state_manager.h
 *
 *  Created on: Nov 16, 2023
 *      Author: mkadayifci
 */

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#define STATE_MANAGER_AD_TYPE_COMPLETE_LOCAL_NAME             0x09

void state_manager_initialize(void);
uint8_t state_manager_is_scribe_mode_enabled(void);
void state_manager_commit_to_flash(void);
void state_manager_set_is_scribe_mode_enabled(uint8_t new_value);
uint8_t state_manager_activity_threshold_low(void);
void state_manager_set_activity_threshold_low(uint8_t new_value);
void state_manager_set_activity_time(uint8_t new_value);
void state_manager_check_and_set_defaults_to_flash(void);
uint8_t * state_manager_server_serial_number(void);
uint8_t * state_manager_server_local_name(void);
uint8_t state_manager_activity_time(void);

#endif /* STATE_MANAGER_H_ */
