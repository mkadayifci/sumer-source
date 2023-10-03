/*
 * scribe.h
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */
#include "stdio.h"

#ifndef SERVICES_INCLUDE_SCRIBE_H_
#define SERVICES_INCLUDE_SCRIBE_H_


#define SCRIBE_LOG_PERIOD_IN_SEC		10 	// 	60
#define SCRIBE_COOLDOWN_PERIOD_IN_SEC	30 // 	300

void scribe_start(void);
void scribe_stop(void);
void scribe_set_scribe_mode(void);
uint8_t scribe_is_in_scribe_mode(void) ;
void scribe_write_seismic_activity_page(void);
uint8_t scribe_is_in_cooldown_period(void);
void scribe_cooldown_period_tick(void);

#endif /* SERVICES_INCLUDE_SCRIBE_H_ */
