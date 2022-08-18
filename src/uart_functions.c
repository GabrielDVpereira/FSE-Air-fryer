#include "uart_functions.h"
#include "config.h"
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

   printf("Mensagem Escrita! \n");

}


void sendIntUart(int uartStream, int data){

    unsigned char dataByte[4]; 
    memcpy(dataByte, &data, 4);

    MODBUS_MESSAGE message = getSendMessageModbus(SEND_INT, dataByte, 4);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);
}


void sendFloatUart(int uartStream, float data){

   unsigned char dataByte[4]; 
   memcpy(dataByte, &data, 4);

   MODBUS_MESSAGE message = getSendMessageModbus(SEND_FLOAT, dataByte, 4);
   writeUart(uartStream, message.buffer, message.size);
   free(message.buffer);
}

void sendStringUart(int uartStream, char* data){
    int dataSize = strlen(data);
    int dataByteSize = dataSize + 1;
    unsigned char* dataByte = (unsigned char*)malloc(dataByteSize); 
    dataByte[0] = dataSize;
    memcpy(&dataByte[1], data, dataSize);
    MODBUS_MESSAGE message = getSendMessageModbus(SEND_STRING, dataByte, dataByteSize);

    writeUart(uartStream, message.buffer, message.size );
    free(dataByte);
    free(message.buffer);
}

// TODO: VERIFICAR CRC 
void readInd(int uartStream){
    MODBUS_MESSAGE message = getRequestMessageModbus(REQUEST_INT);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);

    sleep(1);

    unsigned char buffer[9]; 
    int data; 
    int length = read(uartStream, buffer, 9);
    printBuffer(buffer);
    memcpy(&data, &buffer[3], 4); // copying code 
     
    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %d\n", length, data);

}

// TODO: VERIFICAR CRC 
void readFloat(int uartStream){
    MODBUS_MESSAGE message = getRequestMessageModbus(REQUEST_FLOAT);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);

    sleep(1);

    unsigned char buffer[9]; 
    float data; 
    int length = read(uartStream, buffer, 9);
    printBuffer(buffer);
    memcpy(&data, &buffer[3], 4); // copying code 

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %f\n", length, data);
}

// TODO: VERIFICAR CRC 
void readString(int uartStream){

    MODBUS_MESSAGE message = getRequestMessageModbus(REQUEST_STRING);
    writeUart(uartStream, message.buffer, message.size);
    free(message.buffer);

    sleep(1);
    
    unsigned char buffer[255]; 
    int size = read(uartStream, buffer, 255);
    int messageSize = buffer[3];

    if(size < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("Tamanho da String: %d\n", messageSize); 

    char* str = (char*)malloc(messageSize);
    memcpy(str, &buffer[4], messageSize); 
    if(messageSize < 0){
        printf("Erro na leitura.\n"); 
        return;
    }

    if(messageSize == 0){
        printf("Não há dados disponíveis.\n"); 
        return;
    }

    printf("%i Bytes lidos : %s\n", messageSize, str);

    free(str);

}

