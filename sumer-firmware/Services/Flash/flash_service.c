
#include "hal_types.h"
#include "SPI_Service.h"
#include "flash_service.h"
#define PORT





void storage_initialize(){

}


/**
 * @brief  Writes 256 bytes to next available flash page
 * @param  *buffer: 256 byte array to write
 * @param  temperature: temperature data for page
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
ErrorStatus storage_write_acceleration_page(uint8_t *buffer,uint8_t temperature){
	uint32_t page_address=storage_get_next_page();
	uint8_t bytes_to_send[260];
	bytes_to_send[0]=FLASH_SPI_COMMAND_WRITE;
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
	if(ret ==SUCCESS){
		storage_set_page_metadata(1);
		storage_increase_next_page_value();
	}
	return ret;
}

void storage_write_bytes(uint32_t flash_chip_address,uint8_t *buffer,uint8_t length){

	ErrorStatus ret = spi_service_write_data(
							SPI_DEVICE_ID_FLASH,
							 (uint8_t[]) {
									FLASH_SPI_COMMAND_WRITE,
									flash_chip_address>>16 & 0xFF,
									flash_chip_address>>8 & 0xFF,
									flash_chip_address & 0xFF,
									},
							4,
							buffer,
							length);
}



static void storage_format_flash_chip(){
	//bütün chip'i sil.

	spi_service_write(SPI_DEVICE_ID_FLASH, (uint8_t[]) {
		0xC7,
		0x94,
		0x80,
		0x9A,
	}, 4);

	//sector 0b ilk 4 byte'a başlangıç sayfasını yaz.


}

static uint32_t storage_get_next_page(){

	uint32_t next_page_addr=0x00;
	uint32_t next_page_address_flash_addr= STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE;

	spi_service_read_data(SPI_DEVICE_ID_FLASH, (uint8_t *)&next_page_addr, (uint8_t[]) {
		STORAGE_OPCODE_MAIN_MEMORY_READ,
		STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE>>16 & 0xFF,
		STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE>>8 & 0xFF,
		STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE & 0xFF,
		SPI_DUMMY,SPI_DUMMY,SPI_DUMMY,SPI_DUMMY},8, 4);

	if(next_page_addr==0x00 || next_page_addr==0xFFFFFFFF){
		next_page_addr=STORAGE_ADDR_START_PAGE_OF_ACCELERATION_LOG;
		storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t *)&next_page_addr,4);
	}


	return next_page_addr;

}


static void storage_increase_next_page_value(){
	uint32_t increased_next_page_addr=storage_get_next_page()+0x100; //Last byte is page position. So wee add 255+1 to increase page addr
	storage_write_bytes(STORAGE_FLASH_CHIP_ADDR_NEXT_PAGE,(uint8_t *)&increased_next_page_addr,4);
}


static void storage_set_page_metadata(uint8_t temperature){

}
