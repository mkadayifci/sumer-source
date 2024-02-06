/*
 * flash_storage.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef FLASH_STORAGE_H_
#define FLASH_STORAGE_H_

#include "stdint.h"
#include "hal_types.h"

#define FLASH_STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG		0x000C0000 //3. sector first page
#define FLASH_STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG		0x00800000 //last page address
//#define STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG		0x000EA800 //last page address (Small Size)

#define FLASH_STORAGE_FLASH_CHIP_ADDR_METADATA_BASE			0x00000800 //Start of Sector 0b

#define FLASH_STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE					0x00000000 //0a sector  first page, base+0 4 byte
#define FLASH_STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED		0x00000004 //0a sector  base+4, 1 byte
#define FLASH_STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID	0x00000005 //0a sector  base+5, 2 byte
#define FLASH_STORAGE_FLASH_CHIP_ADDR_ACTIVITY_THRESHOLD_VAL		0x00000007 //0a sector  base+7, 1 byte
#define FLASH_STORAGE_FLASH_CHIP_ADDR_ACTIVITY_TIME_VAL			0x00000008 //0a sector  base+8, 1 byte
#define FLASH_STORAGE_FLASH_CHIP_ADDR_SERIAL_NUMBER				0x00000009 //0a sector  base+9, 6 byte

#define FLASH_STORAGE_DEFAULTS_ACTIVITY_THRESHOLD_VAL		0x14; 	//20 mg
#define FLASH_STORAGE_DEFAULTS_ACTIVITY_TIME_VAL			0x5; 	//5 samples



#define FLASH_STORAGE_OPCODE_WRITE_BUFFER1_WITHOUT_ERASE		0x02
#define FLASH_STORAGE_OPCODE_WRITE_BUFFER1_WITH_ERASE			0x82
#define FLASH_STORAGE_OPCODE_WRITE_BYTE_BUFFER1				0x58

#define FLASH_STORAGE_OPCODE_MAIN_MEMORY_READ					0xD2
#define FLASH_STORAGE_OPCODE_BUFFER_2_READ_HIGH_FREQ			0xD6
#define FLASH_STORAGE_OPCODE_BUFFER_1_READ_LOW_FREQ			0xD1

#define FLASH_STORAGE_OPCODE_STATUS							0xD7

#define FLASH_STORAGE_OPCODE_WRITE_DEFAULT					(FLASH_STORAGE_OPCODE_WRITE_BYTE_BUFFER1)
#define FLASH_STORAGE_OPCODE_READ_DEFAULT					(FLASH_STORAGE_OPCODE_MAIN_MEMORY_READ)


#define FLASH_STORAGE_TOTAL_ACCELARATION_LOG_PAGE_COUNT		29696


#define FLASH_STORAGE_SECTOR_0A			0xFF
#define FLASH_STORAGE_SECTOR_0B			0xFE
#define FLASH_STORAGE_SECTOR_1 			0x01
#define FLASH_STORAGE_SECTOR_2 			0x02
#define FLASH_STORAGE_SECTOR_3 			0x03
#define FLASH_STORAGE_SECTOR_4 			0x04
#define FLASH_STORAGE_SECTOR_5 			0x05
#define FLASH_STORAGE_SECTOR_6 			0x06
#define FLASH_STORAGE_SECTOR_7 			0x07
#define FLASH_STORAGE_SECTOR_8 			0x08
#define FLASH_STORAGE_SECTOR_9 			0x09
#define FLASH_STORAGE_SECTOR_10 		0x0A
#define FLASH_STORAGE_SECTOR_11 		0x0B
#define FLASH_STORAGE_SECTOR_12 		0x0C
#define FLASH_STORAGE_SECTOR_13 		0x0D
#define FLASH_STORAGE_SECTOR_14 		0x0E
#define FLASH_STORAGE_SECTOR_15 		0x0F
#define FLASH_STORAGE_SECTOR_16 		0x10
#define FLASH_STORAGE_SECTOR_17 		0x11
#define FLASH_STORAGE_SECTOR_18 		0x12
#define FLASH_STORAGE_SECTOR_19 		0x13
#define FLASH_STORAGE_SECTOR_20 		0x14
#define FLASH_STORAGE_SECTOR_21 		0x15
#define FLASH_STORAGE_SECTOR_22 		0x16
#define FLASH_STORAGE_SECTOR_23 		0x17
#define FLASH_STORAGE_SECTOR_24 		0x18
#define FLASH_STORAGE_SECTOR_25 		0x19
#define FLASH_STORAGE_SECTOR_26 		0x1A
#define FLASH_STORAGE_SECTOR_27 		0x1B
#define FLASH_STORAGE_SECTOR_28 		0x1C
#define FLASH_STORAGE_SECTOR_29 		0x1D
#define FLASH_STORAGE_SECTOR_30 		0x1E
#define FLASH_STORAGE_SECTOR_31 		0x1F

void flash_storage_enter_deep_sleep_mode(void);
void flash_storage_exit_deep_sleep_mode(void);
void flash_storage_delay(void);
void flash_storage_mini_delay(void);
void flash_storage_wait_until_flash_available(void);
uint8_t flash_storage_is_device_ready(void);
ErrorStatus flash_storage_write_page(uint32_t page_address,uint8_t * buffer);
ErrorStatus flash_storage_read_bytes(uint32_t flash_chip_address,uint8_t * buffer,uint16_t length);
ErrorStatus flash_storage_write_bytes(uint32_t flash_chip_address,uint8_t* buffer,uint16_t length);
void flash_storage_use_256_byte_page(void);
void flash_storage_format_flash_chip(void);
void flash_storage_erase_sector(uint8_t sector);


#endif /* FLASH_STORAGE_H_ */
