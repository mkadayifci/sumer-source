/*
 * scribe.h
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */
#include "stdio.h"


#ifndef SERVICES_INCLUDE_SCRIBE_H_
#define SERVICES_INCLUDE_SCRIBE_H_


#define SCRIBE_LOG_PERIOD_IN_SEC		20 	// 	60
#define SCRIBE_COOLDOWN_PERIOD_IN_SEC	10 // 	300

void scribe_start(void);
void scribe_stop(void);
void scribe_set_scribe_mode(void);
uint8_t scribe_is_false_positive(void);
uint8_t scribe_is_in_scribe_mode(void) ;
void scribe_write_seismic_activity_page(void);
uint8_t scribe_is_in_cooldown_period(void);
uint8_t scribe_is_log_window_over(void);
void scribe_tick(void);
uint8_t scribe_apply_false_positive_filter(uint8_t * pBuffer, uint16_t length);
double scribe_calculate_standart_deviation(const int16_t pData[],uint16_t start , uint16_t length);

#endif /* SERVICES_INCLUDE_SCRIBE_H_ */
