
#include "hal_types.h"
#include "SPI_Service.h"
#include "flash_service.h"
#include "sumer_clock.h"
#include "time.h"
#include "accelerometer.h"
#include "scribe.h"
#define PORT
#define MAX_SEISMIC_LOG_GROUP_ID	0x7FFF


void storage_delay()
{
	for (uint32_t i = 0; i < 50000; i++)__NOP();
}

void storage_mini_delay()
{
	for (uint32_t i = 0; i < 5000; i++)__NOP();
}

void storage_initialize()
{

}

/**
 * @brief  Writes 256 bytes to next available flash page
 * @param  *buffer: 256 byte array to write
 * @param  temperature: temperature data for page
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */

ErrorStatus storage_write_acceleration_page(uint8_t * buffer,uint8_t is_first_page)
{
	uint32_t page_address=storage_get_next_page();
	uint8_t bytes_to_send[260];
	uint16_t seismic_log_group_id=0;
	bytes_to_send[0]=STORAGE_OPCODE_WRITE_DEFAULT;
	bytes_to_send[1]=page_address>>16& 0xFF;
	bytes_to_send[2]=page_address>>8 & 0xFF;
	bytes_to_send[3]=page_address & 0xFF;

	for(int i =0;i<256;i++)
	{
		bytes_to_send[i+4]=buffer[i];
	}
	ErrorStatus ret = spi_service_write(
							SPI_DEVICE_ID_FLASH,
							(uint8_t *)&bytes_to_send,
							260);
	storage_wait_until_flash_available();


	if(ret ==SUCCESS){

		seismic_log_group_id = storage_get_last_seismic_log_group_id_from_flash();
		if(is_first_page)
		{
			seismic_log_group_id++;
			if(seismic_log_group_id==0 || seismic_log_group_id>MAX_SEISMIC_LOG_GROUP_ID)
			{
				seismic_log_group_id=1;
			}
			storage_write_last_seismic_log_group_id_to_flash(seismic_log_group_id);

		}

		uint32_t time_epoch=sumer_clock_get_epoch();

		if(scribe_is_false_positive()){
			time_epoch+=315576000;
		}

		uint8_t temp_H=accelerometer_spi_read_single(ADXL362_REG_TEMP_H);
		uint8_t temp_L=accelerometer_spi_read_single(ADXL362_REG_TEMP_L);
		uint16_t seismic_log_group_id_value = seismic_log_group_id;
		if(is_first_page)
		{
			seismic_log_group_id_value |= (uint16_t)(1 <<15);
		}
		storage_set_page_metadata(page_address,time_epoch,seismic_log_group_id_value,temp_H,temp_L);
		storage_increase_next_page_value(page_address);
	}
	return ret;
}



void storage_wait_until_flash_available()
{
	storage_delay();
	while(!storage_is_device_ready())
	{
			storage_delay();
	}
}

void storage_write_bytes(uint32_t flash_chip_address,uint8_t* buffer,uint16_t length)
{
	ErrorStatus ret = spi_service_write_data(
							SPI_DEVICE_ID_FLASH,
							 (uint8_t[]) {
									STORAGE_OPCODE_WRITE_DEFAULT,
									flash_chip_address>>16 & 0xFF,
									flash_chip_address>>8 & 0xFF,
									flash_chip_address & 0xFF,
									},
							4,
							buffer,
							length);

	storage_wait_until_flash_available();
}

uint8_t storage_is_device_ready()
{
	uint8_t read_buffer[2];
	ErrorStatus ret = spi_service_read_data(
							SPI_DEVICE_ID_FLASH,
							(uint8_t *)&read_buffer,
							 (uint8_t[]) {
										STORAGE_OPCODE_STATUS
									},
							1,
							2);
	return read_buffer[0] & 0x80;
}

void storage_read_bytes(uint32_t flash_chip_address,uint8_t * buffer,uint16_t length)
{
	ErrorStatus ret = spi_service_read_data(
							SPI_DEVICE_ID_FLASH,
							buffer,

							 (uint8_t[]) {
									STORAGE_OPCODE_READ_DEFAULT,
									flash_chip_address>>16 & 0xFF,
									flash_chip_address>>8 & 0xFF,
									flash_chip_address & 0xFF,
									0x00,0x00,0x00,0x00
									},
							8,
							length);
}

void storage_enter_deep_sleep_mode()
{
	//return;
	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0x79
	}, 1);


	storage_mini_delay();
}

void storage_resume_deep_sleep_mode()
{
	//return;
	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0xAB
	}, 1);


	storage_mini_delay();


}


void storage_format_flash_chip()
{
	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0xC7,
		0x94,
		0x80,
		0x9A,
	}, 4);

	storage_wait_until_flash_available();
}

void storage_use_256_byte_page()
{
	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0x3D,
		0x2A,
		0x80,
		0xA6,
	}, 4);

	storage_wait_until_flash_available();
}

static uint32_t storage_get_next_page()
{
	uint32_t next_page_addr=storage_get_next_page_from_flash();
	if(next_page_addr==0x00 || next_page_addr==0xFFFFFFFF || next_page_addr>STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG)
	{
		next_page_addr=STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
		storage_write_next_page_to_flash(next_page_addr);
	}
	return next_page_addr;
}

static void storage_increase_next_page_value(uint32_t page_address)
{

	uint32_t increased_next_page_addr=storage_get_next_page_address(page_address);
	storage_write_next_page_to_flash(increased_next_page_addr);

}

static void storage_write_next_page_to_flash(uint32_t next_page_addr){
	storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t[])  {
			next_page_addr & 0xFF,
			next_page_addr>>8 & 0xFF,
			next_page_addr>>16 & 0xFF,
			next_page_addr>>24 & 0xFF
		},4);
}


static uint32_t storage_get_next_page_from_flash()
{
	uint8_t pageAddressBuffer[4];
	storage_read_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t * )&pageAddressBuffer,4);
	uint32_t next_page_addr = 	(uint32_t)pageAddressBuffer[0] |
								(uint32_t)pageAddressBuffer[1] << 8 |
								(uint32_t)pageAddressBuffer[2] << 16 |
								(uint32_t)pageAddressBuffer[3] << 24;
	return next_page_addr;
}

static void storage_write_last_seismic_log_group_id_to_flash(uint16_t last_seismic_log_group_id)
{
	storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID,(uint8_t[])  {
		last_seismic_log_group_id & 0xFF,
		last_seismic_log_group_id>>8 & 0xFF
		},2);
}

static uint16_t storage_get_last_seismic_log_group_id_from_flash()
{
	uint8_t seismic_log_group_id_buffer[2]={0};
	storage_read_bytes(STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID,(uint8_t * )&seismic_log_group_id_buffer,2);
	uint16_t seismic_log_group_id = 	(uint16_t)seismic_log_group_id_buffer[0] |
										(uint16_t)seismic_log_group_id_buffer[1] << 8 ;
	return seismic_log_group_id;
}


void storage_erase_sector(uint8_t sector)
{
	if(sector<=0x1F ) // 1-31
	{
		spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
				0x7C,
				sector<<2,
				0x00,
				0x00,
			}, 4);

	}
	else if(sector == STORAGE_SECTOR_0B)
	{
		spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
					0x7C,
					0x80,
					0x0F,
					0xFF,
				}, 4);
	}
	storage_wait_until_flash_available();
}


uint32_t storage_get_next_page_address(uint32_t page_address)
{
	uint32_t increased_next_page_addr=page_address+0x100; //Last byte is in page position. So wee add 255+1 to increase page addr
	if(increased_next_page_addr>STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG)
	{
		increased_next_page_addr=STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
	}
	return increased_next_page_addr;
}

static void storage_set_page_metadata(uint32_t page_address,uint32_t time_epoch,uint16_t seismic_log_group_id,uint8_t temp_H,uint8_t temp_L)
{
	//Metadata offset -> (page address -3072) * 8
	uint32_t metadata_offset= (((page_address>>8 ) - (STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG>>8))) * 8 ;
	uint32_t metadata_address = (STORAGE_FLASH_CHIP_ADDR_METADATA_BASE) + metadata_offset;

	storage_write_bytes(
		metadata_address,
		(uint8_t[])  {
			time_epoch & 0xFF,
			time_epoch>>8 & 0xFF,
			time_epoch>>16 & 0xFF,
			time_epoch>>24 & 0xFF,
			seismic_log_group_id & 0xFF,
			seismic_log_group_id>>8 & 0xFF,
			temp_H,
			temp_L
		},
		8);
}

void storage_get_page_metadata(uint16_t page_index,uint8_t * buffer)
{
	uint32_t metadata_address = (STORAGE_FLASH_CHIP_ADDR_METADATA_BASE + (page_index*8));
	storage_read_bytes(metadata_address, buffer,8);

}

void storage_get_page_metadata_by_page_address(uint32_t page_address,uint8_t* buffer)
{
	uint16_t page_index = ((page_address >>8) - (STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG>>8) );
	storage_get_page_metadata(page_index, buffer);
}

