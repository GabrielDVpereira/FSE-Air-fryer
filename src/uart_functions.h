#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS

int initUart(char *);
void configUart(int); 
void writeUart(int, char*); 
void closeUart(int); 

void sendIntUart(int, int); 
void sendFloatUart(int, float); 
void sendStringUart(int, char*); 

void readInd(int);
void readFloat(int);
void readString(int);

#endif
