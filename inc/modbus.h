#ifndef MODBUS
#define MODBUS


typedef struct 
{
    unsigned char* buffer;
    int size;

} MODBUS_REQUEST;

MODBUS_REQUEST getRequestBufferModbus(char);

#endif
