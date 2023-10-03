
#include "hal_types.h"
#include "SPI_Service.h"
#include "flash_service.h"
#include "sumer_clock.h"
#include "time.h"
#include "accelerometer.h"
#define PORT


void storage_delay(){
	for (uint32_t i = 0; i < 200000; i++)__NOP();
}


void storage_initialize(){

}

/**
 * @brief  Writes 256 bytes to next available flash page
 * @param  *buffer: 256 byte array to write
 * @param  temperature: temperature data for page
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus storage_write_acceleration_page(uint8_t* buffer,uint8_t temperature){
	//storage_format_flash_chip();

	uint32_t page_address=storage_get_next_page();
	uint8_t bytes_to_send[260];
	bytes_to_send[0]=STORAGE_OPCODE_WRITE_DEFAULT;
	bytes_to_send[1]=page_address>>16& 0xFF;
	bytes_to_send[2]=page_address>>8 & 0xFF;
	bytes_to_send[3]=page_address & 0xFF;

	for(int i =0;i<255;i++){
		bytes_to_send[i+4]=buffer[i];
	}

	ErrorStatus ret = spi_service_write(
							SPI_DEVICE_ID_FLASH,
							(uint8_t *)&bytes_to_send,
							260);

	while(!storage_is_device_ready()){
			storage_delay();
	}

	if(ret ==SUCCESS){
		uint32_t time_epoch=sumer_clock_get_epoch();
		uint8_t temp_H=accelerometer_spi_read_single(ADXL362_REG_TEMP_H);
		uint8_t temp_L=accelerometer_spi_read_single(ADXL362_REG_TEMP_L);
		storage_set_page_metadata(page_address,time_epoch,temp_H,temp_L);
		storage_increase_next_page_value(page_address);
	}
	return ret;
}

void storage_write_bytes(uint32_t flash_chip_address,uint8_t* buffer,uint8_t length){

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

	while(!storage_is_device_ready()){
			storage_delay();
	}
}

uint8_t storage_is_device_ready(){

	uint8_t read_buffer[2];

	ErrorStatus ret = spi_service_read_data(
							SPI_DEVICE_ID_FLASH,
							&read_buffer,
							 (uint8_t[]) {
										STORAGE_OPCODE_STATUS
									},
							1,
							2);




	return read_buffer[0]&0x80;

}
void storage_read_bytes(uint32_t flash_chip_address,uint8_t* buffer,uint8_t length){

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



void storage_format_flash_chip(){
	//bütün chip'i sil.

	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0xC7,
		0x94,
		0x80,
		0x9A,
	}, 4);

}


void storage_use_256_byte_page(){
	//bütün chip'i sil.

	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0x3D,
		0x2A,
		0x80,
		0xA6,
	}, 4);

}

/*typedef struct {
	uint32_t	log_start_epoch;
	uint32_t	log_page_base_address;
	uint16_t	page_count;

} seismic_log_metadata;*/


static uint32_t storage_get_next_page(){

	uint32_t next_page_addr=0x00;
	uint32_t next_page_address_flash_addr= STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE;

	storage_read_bytes(next_page_address_flash_addr,(uint8_t *)&next_page_addr,4);

	if(next_page_addr==0x00 || next_page_addr==0xFFFFFFFF){
		next_page_addr=STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
		storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t *)&next_page_addr,4);
	}

	return next_page_addr;
}

static void storage_increase_next_page_value(uint32_t page_address){

	uint32_t increased_next_page_addr=page_address+0x100; //Last byte is in page position. So wee add 255+1 to increase page addr

	if(increased_next_page_addr>STORAGE_ADDR_END_PAGE_OF_ACCELERATION_LOG){
		increased_next_page_addr=STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
	}

	storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t *)&increased_next_page_addr,4);
}

static void storage_set_page_metadata(uint32_t page_address,uint32_t time_epoch,uint8_t temp_H,uint8_t temp_L){
//Metadata offset -> (page address -3072) * 8
	uint32_t metadata_offset= (((page_address>>8 ) - (STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG>>8))) * 8 ;
	uint32_t metadata_address = (STORAGE_FLASH_CHIP_ADDR_METADATA_BASE) + metadata_offset;
	uint8_t* p_time_epoch= &time_epoch;

	storage_write_bytes(
		metadata_address,
		(uint8_t[])  {
			time_epoch & 0xFF,
			time_epoch>>8 & 0xFF,
			time_epoch>>16 & 0xFF,
			time_epoch>>24 & 0xFF,
			0x00,
			0x00,
			temp_H,
			temp_L
		},
		8);



}

void storage_get_page_metadata(uint16_t page_index,uint8_t* buffer){
	uint32_t metadata_address = (STORAGE_FLASH_CHIP_ADDR_METADATA_BASE + (page_index*8));
	storage_read_bytes(metadata_address, buffer,8);
}
