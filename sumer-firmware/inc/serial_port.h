
#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_



uint8_t BluetoothDeviceInit(void);
void APP_Tick(void);
void send_data_over_ble_serial(uint8_t* data,uint8_t length );
void flush_ble_serial_buffer(void);
void Process_InputData(uint8_t* data_buffer, uint16_t Nb_bytes);

#endif // _SERIAL_PORT_H_
