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
//#define STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG		0x000EA800 //last page address (Small Size)

#define STORAGE_FLASH_CHIP_ADDR_METADATA_BASE			0x00000800 //Start of Sector 0b

#define STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE					0x00000000 //0a sector  first page, base+0 4 byte
#define STORAGE_FLASH_CHIP_ADDR_IS_SEISMIC_LOG_ENABLED		0x00000004 //0a sector  base+4, 1 byte
#define STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID	0x00000005 //0a sector  base+5, 2 byte
#define STORAGE_FLASH_CHIP_ADDR_ACTIVITY_THRESHOLD_VAL		0x00000007 //0a sector  base+7, 1 byte
#define STORAGE_FLASH_CHIP_ADDR_ACTIVITY_TIME_VAL			0x00000008 //0a sector  base+8, 1 byte


#define STORAGE_DEFAULTS_ACTIVITY_THRESHOLD_VAL		0x14; 	//20 mg
#define STORAGE_DEFAULTS_ACTIVITY_TIME_VAL			0x5; 	//5 samples


//TODO:SEISMIC_LOG_GROUP_ID max 2^31 değer alacak. MSB 1 ise başlangıç sayfası olduğu anlaşılacak. Bütün işlemlerde artık ID kullanılacak. Dönüşümü yapmam lazım

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


#define STORAGE_SECTOR_0A		0xFF
#define STORAGE_SECTOR_0B		0xFE
#define STORAGE_SECTOR_1 		0x01
#define STORAGE_SECTOR_2 		0x02
#define STORAGE_SECTOR_3 		0x03
#define STORAGE_SECTOR_4 		0x04
#define STORAGE_SECTOR_5 		0x05
#define STORAGE_SECTOR_6 		0x06
#define STORAGE_SECTOR_7 		0x07
#define STORAGE_SECTOR_8 		0x08
#define STORAGE_SECTOR_9 		0x09
#define STORAGE_SECTOR_10 		0x0A
#define STORAGE_SECTOR_11 		0x0B
#define STORAGE_SECTOR_12 		0x0C
#define STORAGE_SECTOR_13 		0x0D
#define STORAGE_SECTOR_14 		0x0E
#define STORAGE_SECTOR_15 		0x0F
#define STORAGE_SECTOR_16 		0x10
#define STORAGE_SECTOR_17 		0x11
#define STORAGE_SECTOR_18 		0x12
#define STORAGE_SECTOR_19 		0x13
#define STORAGE_SECTOR_20 		0x14
#define STORAGE_SECTOR_21 		0x15
#define STORAGE_SECTOR_22 		0x16
#define STORAGE_SECTOR_23 		0x17
#define STORAGE_SECTOR_24 		0x18
#define STORAGE_SECTOR_25 		0x19
#define STORAGE_SECTOR_26 		0x1A
#define STORAGE_SECTOR_27 		0x1B
#define STORAGE_SECTOR_28 		0x1C
#define STORAGE_SECTOR_29 		0x1D
#define STORAGE_SECTOR_30 		0x1E
#define STORAGE_SECTOR_31 		0x1F

typedef struct {
	uint32_t	log_start_epoch;
	uint32_t	log_page_base_address;
	uint16_t	page_count;

} seismic_log_metadata;


void storage_initialize();
ErrorStatus storage_write_acceleration_page(uint8_t * buffer,uint8_t is_first_page);
uint8_t storage_is_device_ready();
void storage_format_flash_chip();
void storage_erase_sector(uint8_t sector);
uint32_t storage_get_next_page_address(uint32_t page_address);
void storage_use_256_byte_page();
static uint32_t  storage_get_next_page();
void storage_enter_deep_sleep_mode();
void storage_resume_deep_sleep_mode();
static void storage_increase_next_page_value(uint32_t page_address);
static void storage_set_page_metadata(uint32_t page_address,uint32_t time_epoch,uint16_t seismic_log_group_id,uint8_t temp_H,uint8_t temp_L);
void storage_write_bytes(uint32_t  flash_chip_address,uint8_t * buffer,uint16_t length);
void storage_read_bytes(uint32_t flash_chip_address,uint8_t * buffer,uint16_t length);
void storage_get_page_metadata(uint16_t page_index,uint8_t * buffer);
void storage_get_page_metadata_by_page_address(uint32_t page_address,uint8_t * buffer);
static void storage_write_next_page_to_flash(uint32_t next_page_addr);
static void storage_write_last_seismic_log_group_id_to_flash(uint16_t last_seismic_log_group_id);
static uint32_t storage_get_next_page_from_flash();
static uint16_t storage_get_last_seismic_log_group_id_from_flash();
void storage_wait_until_flash_available();
void storage_delay();
void storage_mini_delay();


#endif /* SERVICES_FLASH_FLASH_SERVICE_H_ */
