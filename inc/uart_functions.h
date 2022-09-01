#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS

int initUart(char *);
void configUart(int); 
void writeUart(int, unsigned char*, int); 
void closeUart(int); 

void sendIntUart(int, int, char);
void sendByteUart(int, unsigned char, char);

float readFloat(int, char);
int readInt(int, char);

#endif
