#ifndef MODBUS
#define MODBUS


typedef struct 
{
    unsigned char* buffer;
    int size;
    short CRC; 

} MODBUS_MESSAGE;

#define CRC_ERROR 0
#define CRC_SUCCESS 1
typedef struct 
{
    int error;
    unsigned char data[4];
    unsigned char device_address; 
    unsigned char modbus_code; 
    unsigned char subcode; 
    short crc; 
} MODBUS_RESPONSE;

MODBUS_MESSAGE format_request_message(char);
MODBUS_MESSAGE format_send_message(char subcode, unsigned char* data, int messageSize);

#endif
