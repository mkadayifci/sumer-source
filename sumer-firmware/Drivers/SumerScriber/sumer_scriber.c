/*
 * sumer_scriber.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#include "interrupt_manager.h"
#include "sumer_firmware.h"
#include "sumer_scriber.h"
#include "inertial_sensor.h"
#include "flash_storage.h"
#include "serial_port.h"
#include "omega_speedmaster.h"
#include "math.h"



static scriber_state_t sumer_scriber_state={0};

static volatile uint8_t scribe_accelerometer_FIFO_buffer[256];

scriber_state_t * sumer_scriber_get_state(void)
{
	return (scriber_state_t *)&sumer_scriber_state;
}

void sumer_scriber_start(void)
{
	interrupt_manager_set_mcu_interrupt_pin_state(INTERRUPT_MANAGER_FIFO_WATERMARK_PIN,ENABLE);
	inertial_sensor_enable_fifo_stream();
	sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_SCRIBE);
	sumer_firmware_get_state_object()->waiting_to_write_first_page=SET;
	sumer_scriber_state.scribe_start_time=omega_speedmaster_get_epoch();

}


void sumer_scriber_stop(void)
{
	sumer_scriber_state.cooldown_start_time =omega_speedmaster_get_epoch();
	inertial_sensor_disable_fifo_stream();
	sumer_firmware_set_state_flag(SUMER_FIRMWARE_STATE_SCRIBE_COOLDOWN);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_SCRIBE);
	sumer_firmware_clear_state_flag(SUMER_FIRMWARE_STATE_FIFO_OVERFLOW_OCCURED);

}

uint8_t sumer_scriber_is_log_window_over(void)
{
	return ( omega_speedmaster_get_epoch()- sumer_scriber_state.scribe_start_time ) > SUMER_SCRIBER_LOG_PERIOD_IN_SEC;
}

static void sumer_scriber_set_whole_metadata(uint32_t page_address_to_write)
{
	uint32_t time_epoch = omega_speedmaster_get_epoch();
	uint8_t temp_H = inertial_sensor_get_register(ADXL362_REG_TEMP_H);
	uint8_t temp_L = inertial_sensor_get_register(ADXL362_REG_TEMP_L);



	uint16_t last_seismic_log_group_id =	sumer_scriber_get_last_seismic_log_group_id_from_storage();


	uint16_t seismic_log_group_id_with_flag = last_seismic_log_group_id;

	if(sumer_firmware_get_state_object()->waiting_to_write_first_page==SET)//It is first page for activity
	{
		last_seismic_log_group_id++;
		seismic_log_group_id_with_flag = last_seismic_log_group_id | (uint16_t)(1 <<15);
		sumer_firmware_get_state_object()->waiting_to_write_first_page=RESET;
	}

	sumer_scriber_set_seismic_log_page_metadata(page_address_to_write, time_epoch, seismic_log_group_id_with_flag, temp_H, temp_L);
	sumer_scriber_write_last_seismic_log_group_id_to_storage(last_seismic_log_group_id);

}

void sumer_scriber_write_log_page_from_inertial_sensor_fifo(void)
{
	uint16_t fifo_samples_count=inertial_sensor_get_waiting_fifo_records_lenght();
	if(fifo_samples_count>=256)
	{
		inertial_sensor_read_FIFO((uint8_t * )&scribe_accelerometer_FIFO_buffer, 256);
		uint32_t page_address_to_write =sumer_scriber_get_next_page_address_from_flash();
		flash_storage_write_page(page_address_to_write,(uint8_t * )&scribe_accelerometer_FIFO_buffer);
		sumer_scriber_set_whole_metadata(page_address_to_write);
		sumer_scriber_increase_next_page_address(page_address_to_write);
	}
}

static void sumer_scriber_set_seismic_log_page_metadata(uint32_t written_page_address,uint32_t time_epoch,uint16_t seismic_log_group_id,uint8_t temp_H,uint8_t temp_L)
{
	//Metadata offset -> (page address -3072) * 8
	uint32_t metadata_offset= (((written_page_address>>8 ) - (FLASH_STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG>>8))) * 8 ;
	uint32_t metadata_address = (FLASH_STORAGE_FLASH_CHIP_ADDR_METADATA_BASE) + metadata_offset;

	flash_storage_write_bytes(
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



static uint32_t sumer_scriber_get_next_page_address_from_flash(void)
{
	uint8_t pageAddressBuffer[4];
	flash_storage_read_bytes(FLASH_STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t * )&pageAddressBuffer,4);
	uint32_t next_page_addr = 	(uint32_t)pageAddressBuffer[0] |
								(uint32_t)pageAddressBuffer[1] << 8 |
								(uint32_t)pageAddressBuffer[2] << 16 |
								(uint32_t)pageAddressBuffer[3] << 24;
	if(next_page_addr==0x00 || next_page_addr==0xFFFFFFFF || next_page_addr>FLASH_STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG)
	{
		next_page_addr=FLASH_STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
	}
	return next_page_addr;
}

static void sumer_scriber_increase_next_page_address(uint32_t current_page_address)
{
	uint32_t increased_next_page_addr=sumer_scriber_get_calculated_next_page_address(current_page_address);

	flash_storage_write_bytes(FLASH_STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t[])  {
			increased_next_page_addr & 0xFF,
			increased_next_page_addr>>8 & 0xFF,
			increased_next_page_addr>>16 & 0xFF,
			increased_next_page_addr>>24 & 0xFF
		},4);

}

uint32_t sumer_scriber_get_calculated_next_page_address(uint32_t current_page_address){
	uint32_t increased_next_page_addr=current_page_address+0x100; //Last byte is in page position. So wee add 255+1 to increase page addr
	if(increased_next_page_addr>FLASH_STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG)
	{
		increased_next_page_addr=FLASH_STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
	}
	return increased_next_page_addr;
}

uint8_t sumer_scriber_is_in_cooldown_period(void)
{
	long current_epoch_time = omega_speedmaster_get_epoch();
	if ((current_epoch_time - sumer_scriber_state.cooldown_start_time ) < SUMER_SCRIBER_COOLDOWN_PERIOD_IN_SEC)
	{
		return 1;
	}
	return 0;
}



static void sumer_scriber_write_last_seismic_log_group_id_to_storage(uint16_t last_seismic_log_group_id)
{
	flash_storage_write_bytes(FLASH_STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID,(uint8_t[])  {
		last_seismic_log_group_id & 0xFF,
		last_seismic_log_group_id>>8 & 0xFF
		},2);
}

static uint16_t sumer_scriber_get_last_seismic_log_group_id_from_storage(void)
{
	uint8_t seismic_log_group_id_buffer[2]={0};
	flash_storage_read_bytes(FLASH_STORAGE_FLASH_CHIP_ADDR_LAST_SEISMIC_LOG_GROUP_ID,(uint8_t * )&seismic_log_group_id_buffer,2);
	uint16_t seismic_log_group_id = 	(uint16_t)seismic_log_group_id_buffer[0] |
										(uint16_t)seismic_log_group_id_buffer[1] << 8 ;

	if(seismic_log_group_id==0 || seismic_log_group_id>MAX_SEISMIC_LOG_GROUP_ID)
	{
		seismic_log_group_id=1;
	}

	return seismic_log_group_id;
}






void sumer_scriber_get_page_metadata(uint16_t page_index,uint8_t * buffer)
{
	uint32_t metadata_address = (FLASH_STORAGE_FLASH_CHIP_ADDR_METADATA_BASE + (page_index*8));
	flash_storage_read_bytes(metadata_address, buffer,8);

}

void sumer_scriber_get_page_metadata_by_page_address(uint32_t page_address,uint8_t* buffer)
{
	uint16_t page_index = ((page_address >>8) - (FLASH_STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG>>8) );
	sumer_scriber_get_page_metadata(page_index, buffer);
}



uint8_t volatile sumer_scriber_apply_false_positive_filter(uint8_t * pBuffer, uint16_t length)
{
	int16_t current_x;
	int16_t current_y;
	int16_t current_z;
	int16_t x_array[42] = {0};
	int16_t y_array[42] = {0};
	int16_t z_array[42] = {0};
	double x_std_dev;
	double y_std_dev;
	double z_std_dev;
	double x_threshold = 3.0;
	double y_threshold = 3.0;
	double z_threshold = 4.0;
	uint8_t array_indexer = 0;
	for (uint16_t i = 0; i < 252; i += 6)
	{
		uint8_t signExtensionBits;
		uint8_t highByte;

		highByte=pBuffer[i+1];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_x = (int16_t)(highByte<<8) |  (int16_t)pBuffer[i];


		highByte=pBuffer[i+3];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_y = (int16_t) (highByte<<8) |  (int16_t)pBuffer[i + 2];


		highByte=pBuffer[i+5];
		signExtensionBits =  highByte & 0x30;
		highByte &= (0x3F);
		highByte |= (uint8_t)(signExtensionBits << 2);
		current_z = (int16_t)(highByte<<8)| (int16_t)pBuffer[i + 4];


		x_array[array_indexer] = current_x;
		y_array[array_indexer] = current_y;
		z_array[array_indexer] = current_z;
		array_indexer++;
	}
	x_std_dev = sumer_scriber_calculate_standart_deviation(x_array,10, 30);
	y_std_dev = sumer_scriber_calculate_standart_deviation(y_array,10, 30);
	z_std_dev = sumer_scriber_calculate_standart_deviation(z_array,10, 30);

	if (x_std_dev < x_threshold &&
		y_std_dev < y_threshold &&
		z_std_dev < z_threshold)
	{
		return 1;
	}
	return 0;
}

double sumer_scriber_calculate_standart_deviation(const int16_t pData[],uint16_t start , uint16_t length )
{
	if (length <= 1)
	{
		return 0.0;
	}

	double mean = 0;
	for (int i = start; i < length + start ; i++)
	{
		mean += pData[i];
	}
	mean /= length;

	double variance = 0.0;
	for (int i = start; i < length + start; i++)
	{
		double diff = pData[i] - mean;
		variance += diff * diff;
	}
	variance /= length;
	double standardDeviation = sqrt(variance);
	return standardDeviation;
}
