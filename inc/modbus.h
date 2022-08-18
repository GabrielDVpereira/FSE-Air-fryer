#ifndef MODBUS
#define MODBUS


typedef struct 
{
    unsigned char* buffer;
    int size;

} MODBUS_MESSAGE;

MODBUS_MESSAGE getRequestMessageModbus(char);
MODBUS_MESSAGE getSendMessageModbus(char subcode, unsigned char* data, int messageSize);

#endif
