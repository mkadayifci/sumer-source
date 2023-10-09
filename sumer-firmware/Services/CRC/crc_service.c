#include "stdio.h"
#include "stdint.h"
#include "crc_service.h"


static uint32_t crc_table[256];


void crc_service_create_crc_table() {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC_POLYNOMIAL;
            } else {
                crc >>= 1;
            }
        }
        crc_table[i] = crc;
    }
}

uint32_t crc_service_calculate_crc(const uint8_t *data, size_t length) {
    uint32_t crc = 0xFFFFFFFFu;  // Başlangıç değeri
    for (size_t i = 0; i < length; i++) {
        uint8_t index = (uint8_t)((crc ^ data[i]) & 0xFFu);
        crc = (crc >> 8) ^ crc_table[index];
    }
    return crc ^ 0xFFFFFFFFu;  // Sonuçun tersini al
}

/*int main() {
    create_crc_table();

    // Hesaplama için kullanılacak veri
    uint8_t data[] = {0x01, 0x23, 0x45, 0x67, 0x89};
    size_t data_length = sizeof(data) / sizeof(data[0]);

    // CRC hesapla
    uint32_t crc_result = calculate_crc(data, data_length);

    printf("CRC: 0x%08X\n", crc_result);

    return 0;
}*/
