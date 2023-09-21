/*
 * flash_service.h
 *
 *  Created on: Jul 30, 2023
 *      Author: mkadayifci
 */

#ifndef SERVICES_FLASH_FLASH_SERVICE_H_
#define SERVICES_FLASH_FLASH_SERVICE_H_




#define STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG		0x000C0000 //3. sector first page
#define STORAGE_FLASH_CHIP_ADDR_METADATA_BASE			0x00000900 //Start of Sector 0b

#define STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE				0x00000000 //0a sector  first page, first 4 byte


#define STORAGE_OPCODE_WRITE_BUFFER1_WITHOUT_ERASE		0x02
#define STORAGE_OPCODE_WRITE_BUFFER1_WITH_ERASE			0x82
#define STORAGE_OPCODE_WRITE_BYTE_BUFFER1				0x58

#define STORAGE_OPCODE_WRITE_DEFAULT					(STORAGE_OPCODE_WRITE_BYTE_BUFFER1)

#define STORAGE_OPCODE_MAIN_MEMORY_READ					0xD2

void storage_initialize();
ErrorStatus storage_write_acceleration_page(uint8_t *buffer,uint8_t temperature);
static void storage_format_flash_chip();
static uint32_t storage_get_next_page();
static void storage_increase_next_page_value(uint32_t page_address);
static void storage_set_page_metadata(uint8_t temperature,uint32_t page_address);
void storage_write_bytes(uint32_t  flash_chip_address,uint8_t *buffer,uint8_t length);


#endif /* SERVICES_FLASH_FLASH_SERVICE_H_ */