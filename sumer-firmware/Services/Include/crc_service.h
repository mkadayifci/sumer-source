/*
 * crc_service.h
 *
 *  Created on: Oct 4, 2023
 *      Author: mkadayifci
 */

#ifndef SERVICES_INCLUDE_CRC_SERVICE_H_
#define SERVICES_INCLUDE_CRC_SERVICE_H_

#define CRC_POLYNOMIAL 0xEDB88320u



void crc_service_create_crc_table();
uint32_t crc_service_calculate_crc(const uint8_t *data, size_t length) ;

#endif /* SERVICES_INCLUDE_CRC_SERVICE_H_ */
