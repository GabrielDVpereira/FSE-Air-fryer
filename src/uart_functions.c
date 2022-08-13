#include "uart_functions.h"
#include "config.h"
#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>

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

void writeUart(int uartStream, unsigned char* info) {
   int response = write(uartStream, info, sizeof(info));
   if(response < 0){
    printf("Erro ao escrever na UART\n"); 
    return; 
   } 

   printf("Mensagem Escrita! \n");

}


unsigned char matricula[] = {0, 3, 4, 1}; 


void printBuffer(unsigned char * buffer) {
    for(int i=0; i < 9; i++){
    printf("%x", buffer[i]);
    }
    printf("\n");
}

void sendIntUart(int uartStream, int data){
    unsigned char buffer[10];
    buffer[0] = SEND_INT;
    memcpy(&buffer[1], &data, sizeof(data)); // copying info to send
    memcpy(&buffer[5],  matricula, sizeof(matricula)); // copying matricula
    printf("Buffer para envio de inteiro criado! \n"); 
    printBuffer(buffer);
    writeUart(uartStream, buffer);
}


void sendFloatUart(int uartStream, float data){
    unsigned char buffer[20];
    buffer[0] = SEND_FLOAT;
    memcpy(&buffer[1], &data, sizeof(data)); // copying info to send
    memcpy(&buffer[5], matricula, sizeof(matricula)); // copying matricula
   
   printf("Buffer para envio de float criado! \n"); 
   printBuffer(buffer);
   writeUart(uartStream, buffer);
}

void sendStringUart(int uartStream, char* data){
    unsigned char buffer[255];
    int data_size = strlen(data);
    buffer[0] = SEND_STRING;
    buffer[1] = data_size; 
    memcpy(&buffer[2], data, data_size); // copying data 
    memcpy(&buffer[2 + data_size], matricula, sizeof(matricula)); // copying matricula 

    printf("Buffer para envio de string criado! \n"); 
    printBuffer(buffer);
    writeUart(uartStream, buffer);
}

void readInd(int uartStream){
    unsigned char cmd_buffer[20];
    cmd_buffer[0] =  REQUEST_INT;

    memcpy(&cmd_buffer[1], matricula, sizeof(matricula)); // copying matricula
    writeUart(uartStream, cmd_buffer);

    sleep(1);

    unsigned char buffer[20]; 
    int data; 
    int length = read(uartStream, (void*)buffer, 4);
    memcpy(&data, &buffer, 4); // copying code 
     
    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %d\n", length, data);

}

void readFloat(int uartStream){
    float buffer; 
    int length = read(uartStream, (void*)&buffer, 4);

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %f\n", length, buffer);
}

void readString(int uartStream){
    int message_len; 
    int size = read(uartStream, (void*)&message_len, 1);

    if(size < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("Tamanho da String: %d\n", message_len); 

    unsigned char buffer[255];  
    int message_total_size = read(uartStream, (void*)buffer, message_len);

    if(message_total_size < 0){
        printf("Erro na leitura.\n"); 
        return;
    }

    if(message_total_size == 0){
        printf("Não há dados disponíveis.\n"); 
        return;
    }

    buffer[message_total_size] = '\0';
    printf("%i Bytes lidos : %s\n", message_total_size, buffer);

}

