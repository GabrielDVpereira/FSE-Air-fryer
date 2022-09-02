#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS
#include "modbus.h"

int init_uart(char *);
void config_uart(int); 
void write_uart(int, unsigned char*, int); 
void close_uart(int); 

void send_int_uart(int, int, char);
void send_float_uart(int, float, char);
void send_byte_uart(int, unsigned char, char);

float read_float(int, char);
MODBUS_RESPONSE read_int(int, char);

#endif
