/*
 * flash_storage.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */
#include "sumer_spi_driver.h"
#include "flash_storage.h"

void flash_storage_enter_deep_sleep_mode(void)
{
	sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {0x79}, 1);
	flash_storage_delay();
}

void flash_storage_exit_deep_sleep_mode(void)
{
	sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {0xAB}, 1);
	flash_storage_delay();
}

void flash_storage_wait_until_flash_available(void)
{
	flash_storage_delay();
	while(!flash_storage_is_device_ready())
	{
		flash_storage_delay();
	}
}

uint8_t flash_storage_is_device_ready(void)
{
	uint8_t read_buffer[2];
	ErrorStatus ret = sumer_spi_driver_read_data(
							SPI_DEVICE_ID_FLASH,
							(uint8_t *)&read_buffer,
							 (uint8_t[]) {
										FLASH_STORAGE_OPCODE_STATUS
									},
							1,
							2);
	return read_buffer[0] & 0x80;
}

ErrorStatus flash_storage_read_bytes(uint32_t flash_chip_address,uint8_t * buffer,uint16_t length)
{
	return sumer_spi_driver_read_data(
							SPI_DEVICE_ID_FLASH,
							buffer,

							 (uint8_t[]) {
									FLASH_STORAGE_OPCODE_READ_DEFAULT,
									flash_chip_address>>16 & 0xFF,
									flash_chip_address>>8 & 0xFF,
									flash_chip_address & 0xFF,
									0x00,0x00,0x00,0x00
									},
							8,
							length);
}

ErrorStatus flash_storage_write_bytes(uint32_t flash_chip_address,uint8_t* buffer,uint16_t length)
{
	ErrorStatus ret = sumer_spi_driver_write_data(
							SPI_DEVICE_ID_FLASH,
							 (uint8_t[]) {
									FLASH_STORAGE_OPCODE_WRITE_DEFAULT,
									flash_chip_address>>16 & 0xFF,
									flash_chip_address>>8 & 0xFF,
									flash_chip_address & 0xFF,
									},
							4,
							buffer,
							length);
	flash_storage_wait_until_flash_available();
	return ret;
}

void flash_storage_format_flash_chip(void)
{
	sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {0xC7,0x94,0x80,0x9A}, 4);
	flash_storage_wait_until_flash_available();
}

void flash_storage_use_256_byte_page(void)
{
	sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {0x3D,0x2A,0x80,0xA6}, 4);
	flash_storage_wait_until_flash_available();
}

ErrorStatus flash_storage_write_page(uint32_t page_address,uint8_t * buffer)
{
	uint8_t bytes_to_send[260];
	bytes_to_send[0]=FLASH_STORAGE_OPCODE_WRITE_DEFAULT;
	bytes_to_send[1]=page_address>>16& 0xFF;
	bytes_to_send[2]=page_address>>8 & 0xFF;
	bytes_to_send[3]=page_address & 0xFF;

	for(int i =0;i<256;i++)
	{
		bytes_to_send[i+4]=buffer[i];
	}
	ErrorStatus ret=sumer_spi_driver_write(
							SPI_DEVICE_ID_FLASH,
							(uint8_t *)&bytes_to_send,
							260);
	flash_storage_wait_until_flash_available();

	return ret;
}

void flash_storage_delay(void)
{
	for (uint32_t i = 0; i < 50000; i++)__NOP();
}

void flash_storage_mini_delay(void)
{
	for (uint32_t i = 0; i < 5000; i++)__NOP();
}

void flash_storage_erase_sector(uint8_t sector)
{
	if(sector<=0x1F ) // 1-31
	{
		sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
				0x7C,
				sector<<2,
				0x00,
				0x00,
			}, 4);

	}
	else if(sector == FLASH_STORAGE_SECTOR_0B)
	{
		sumer_spi_driver_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
					0x7C,
					0x00,
					0x08,
					0x06,
				}, 4);
	}
	flash_storage_wait_until_flash_available();
}
