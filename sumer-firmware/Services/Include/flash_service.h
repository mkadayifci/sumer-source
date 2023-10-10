/*
 * flash_service.h
 *
 *  Created on: Jul 30, 2023
 *      Author: mkadayifci
 */
#include "hal_types.h"
#include "sumer_clock.h"
#include "stddef.h"

#ifndef SERVICES_FLASH_FLASH_SERVICE_H_
#define SERVICES_FLASH_FLASH_SERVICE_H_




#define STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG		0x000C0000 //3. sector first page
#define STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG		0x00800000 //last page address

#define STORAGE_FLASH_CHIP_ADDR_METADATA_BASE			0x00000800 //Start of Sector 0b

#define STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE				0x00000000 //0a sector  first page, base+0 4 byte
#define STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED	0x00000004 //0a sector  base+4, 2 byte


#define STORAGE_OPCODE_WRITE_BUFFER1_WITHOUT_ERASE		0x02
#define STORAGE_OPCODE_WRITE_BUFFER1_WITH_ERASE			0x82
#define STORAGE_OPCODE_WRITE_BYTE_BUFFER1				0x58

#define STORAGE_OPCODE_MAIN_MEMORY_READ					0xD2
#define STORAGE_OPCODE_BUFFER_2_READ_HIGH_FREQ			0xD6
#define STORAGE_OPCODE_BUFFER_1_READ_LOW_FREQ			0xD1

#define STORAGE_OPCODE_STATUS							0xD7

#define STORAGE_OPCODE_WRITE_DEFAULT					(STORAGE_OPCODE_WRITE_BYTE_BUFFER1)
#define STORAGE_OPCODE_READ_DEFAULT						(STORAGE_OPCODE_MAIN_MEMORY_READ)


#define STORAGE_TOTAL_ACCELARATION_LOG_PAGE_COUNT		29696



typedef struct {
	uint32_t	log_start_epoch;
	uint32_t	log_page_base_address;
	uint16_t	page_count;

} seismic_log_metadata;

//TODO: length değişkenlerini size_t ile değiştir
void storage_initialize();
ErrorStatus storage_write_acceleration_page(uint8_t * buffer);
uint8_t storage_is_device_ready();
void storage_format_flash_chip();
uint32_t storage_get_next_page_address(uint32_t page_address);
void storage_use_256_byte_page();
static uint32_t  storage_get_next_page();
static void storage_increase_next_page_value(uint32_t page_address);
static void storage_set_page_metadata(uint32_t page_address,uint32_t time_epoch,uint8_t temp_H,uint8_t temp_L);
void storage_write_bytes(uint32_t  flash_chip_address,uint8_t * buffer,size_t length);
void storage_read_bytes(uint32_t flash_chip_address,uint8_t * buffer,size_t length);
void storage_get_page_metadata(uint16_t page_index,uint8_t * buffer);
void storage_get_page_metadata_by_page_address(uint32_t page_address,uint8_t * buffer);

#endif /* SERVICES_FLASH_FLASH_SERVICE_H_ */
