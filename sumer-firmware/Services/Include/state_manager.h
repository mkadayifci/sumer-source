/*
 * state_manager.h
 *
 *  Created on: Nov 16, 2023
 *      Author: mkadayifci
 */

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

void state_manager_initialize(void);
uint8_t state_manager_is_scribe_mode_enabled(void);
void state_manager_set_is_scribe_mode_enabled(uint8_t new_value);


#endif /* STATE_MANAGER_H_ */
