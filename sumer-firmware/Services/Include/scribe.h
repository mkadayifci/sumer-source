/*
 * scribe.h
 *
 *  Created on: Aug 8, 2023
 *      Author: mkadayifci
 */

#ifndef SERVICES_INCLUDE_SCRIBE_H_
#define SERVICES_INCLUDE_SCRIBE_H_


#define SCRIBE_LOG_PERIOD_IN_SEC	60

void scribe_start(void);
void scribe_stop(void);
void scribe_set_scribe_mode(void);
void scribe_write_seismic_activity_page(void);


#endif /* SERVICES_INCLUDE_SCRIBE_H_ */
