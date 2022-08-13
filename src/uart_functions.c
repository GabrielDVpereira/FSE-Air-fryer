#include "uart_functions.h"
#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "config.c"

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

void writeUart(int uartStream, char* info) {
   int response = write(uartStream, info, sizeof(info));
   if(response < 0){
    printf("Erro ao escrever na UART\n"); 
    return; 
   } 

   printf("Mensagem Escrita! \n");

}


char matricula = {0, 3, 4, 1}; 

void sendIntUart(int uartStream, int data){
    unsigned char buffer[20];
    memcpy(buffer, &SEND_INT, 1); // copying code 
    memcpy(&buffer[1], &data, sizeof(data)); // copying info to send
    memcpy(&buffer[5], matricula, sizeof(matricula)); // copying matricula
   
   printf("Buffer para envio de inteiro criado! \n"); 

   writeUart(uartStream, buffer);
}


void sendFloatUart(int uartStream, float data){
    unsigned char buffer[20];
    memcpy(buffer, &SEND_FLOAT, 1); // copying code 
    memcpy(&buffer[1], &data, sizeof(data)); // copying info to send
    memcpy(&buffer[5], matricula, sizeof(matricula)); // copying matricula
   
   printf("Buffer para envio de inteiro criado! \n"); 

   writeUart(uartStream, buffer);
}

void sendSringUart(int uartStream, char* data){
    unsigned char buffer[255];
    int data_size = strlen(data);

    memcpy(buffer, &SEND_STRING, 1); // copying code 
    memcpy(&buffer[1], &data_size, 1); // copying string size
    memcpy(&buffer[2], data, data_size); // copying data 
    memcpy(&buffer[2 + data_size], matricula, sizeof(matricula)); // copying matricula 
    writeUart(uartStream, buffer);
}

void readInd(int uartStream){
    unsigned int buffer; 
    int length = read(uartStream, (void*)&buffer, 4);

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %d\n", length, buffer);

}

void readFloat(int uartStream){
    float buffer; 
    int length = read(uartStream, (void*)&buffer, 4);

    if(length < 0){
        printf("Erro na leitura\n"); 
        return;
    }

    printf("%i Bytes lidos : %d\n", length, buffer);
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

