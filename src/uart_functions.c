#include "uart_functions.h"
#include "uart_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include "modbus.h"
#include "utils.h"


int initUart(char * path) {
    int uart0_filestream = -1;
    
    uart0_filestream = open(path, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }  
    return uart0_filestream;
}

void configUart(int uartStream){
    struct termios options;
    tcgetattr(uartStream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uartStream, TCIFLUSH); // cleaning buffer
    tcsetattr(uartStream, TCSANOW, &options);
}

void closeUart(int uartStream){
    close(uartStream);
}

void writeUart(int uartStream, unsigned char* info, int size) {
   int response = write(uartStream, info, size);
   if(response < 0){
    printf("Erro ao escrever na UART\n"); 
    return; 
   } 
}


void sendIntUart(int uartStream, int data, char msgType){
    unsigned char dataByte[4]; 
    memcpy(dataByte, &data, 4);

    MODBUS_MESSAGE message = getSendMessageModbus(msgType, dataByte, 4);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);
}

void sendByteUart(int uartStream, unsigned char data, char msgType){
    MODBUS_MESSAGE message = getSendMessageModbus(msgType, &data, 1);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);
}

// TODO: VERIFICAR CRC 
float readFloat(int uartStream, char msgType){
    MODBUS_MESSAGE message = getRequestMessageModbus(msgType);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);

    usleep(500000); 

    unsigned char buffer[9]; 
    float data; 
    int length = read(uartStream, buffer, 9);
    memcpy(&data, &buffer[3], 4); // copying code 

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    return data;
}

// TODO: VERIFICAR CRC 
int readInt(int uartStream, char msgType){
    MODBUS_MESSAGE message = getRequestMessageModbus(msgType);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);

    usleep(500000); 

    unsigned char buffer[9]; 
    int data; 
    int length = read(uartStream, buffer, 9);
    memcpy(&data, &buffer[3], 4); // copying code 

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }
    return data;
}

