#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS
#include "modbus.h"


#define UART_WRITE_ERROR 0
#define UART_WRITE_SUCCESS 1

int init_uart(char *);
void config_uart(int); 
int write_uart(int, unsigned char*, int); 
void close_uart(int); 

void send_int_uart(int, int, char);
void send_float_uart(int, float, char);
void send_byte_uart(int, unsigned char, char);

MODBUS_RESPONSE read_uart(int, char);

#endif
