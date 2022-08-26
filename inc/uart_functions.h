#ifndef UART_FUNCTIONS
#define UART_FUNCTIONS

int initUart(char *);
void configUart(int); 
void writeUart(int, unsigned char*, int); 
void closeUart(int); 

// void sendIntUart(int, int); 
// void sendFloatUart(int, float); 
// void sendStringUart(int, char*); 

float readTemperature(int, char);
void readReferenceTemperature(int);
void readUserInputs(int);

#endif
