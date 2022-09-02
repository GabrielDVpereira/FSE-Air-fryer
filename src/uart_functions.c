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
#include "crc16.h"


int init_uart(char * path) {
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

void config_uart(int uartStream){
    struct termios options;
    tcgetattr(uartStream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uartStream, TCIFLUSH); // cleaning buffer
    tcsetattr(uartStream, TCSANOW, &options);
}

void close_uart(int uartStream){
    close(uartStream);
}

void write_uart(int uartStream, unsigned char* info, int size) {
   int response = write(uartStream, info, size);
   if(response < 0){
    printf("Erro ao escrever na UART\n"); 
    return; 
   } 
}

void send_int_uart(int uartStream, int data, char msgType){
    unsigned char dataByte[4]; 
    memcpy(dataByte, &data, 4);

    MODBUS_MESSAGE message = format_send_message(msgType, dataByte, 4);
    write_uart(uartStream, message.buffer, message.size);
    free(message.buffer);

}

void send_float_uart(int uartStream, float data, char msgType){
    unsigned char dataByte[4]; 
    memcpy(dataByte, &data, 4);

    MODBUS_MESSAGE message = format_send_message(msgType, dataByte, 4);
    write_uart(uartStream, message.buffer, message.size);
    free(message.buffer);
}

void send_byte_uart(int uartStream, unsigned char data, char msgType){
    MODBUS_MESSAGE message = format_send_message(msgType, &data, 1);
    write_uart(uartStream, message.buffer, message.size);
    free(message.buffer);
}

// TODO: VERIFICAR CRC 
float read_float(int uartStream, char msgType){
    MODBUS_MESSAGE message = format_request_message(msgType);
    write_uart(uartStream, message.buffer, message.size);
    free(message.buffer);

    usleep(500000); 

    unsigned char buffer[9]; 
    float data; 
    int length = read(uartStream, buffer, 9);
    memcpy(&data, &buffer[3], 4); // copying code 

    if(length < 0){
        printf("Erro na leitura read_float\n"); 
        return;
    }
    return data;
}

// TODO: VERIFICAR CRC 
MODBUS_RESPONSE read_int(int uartStream, char msgType){
    MODBUS_MESSAGE message = format_request_message(msgType);
    write_uart(uartStream, message.buffer, message.size);
    free(message.buffer);

    usleep(500000); 

    MODBUS_RESPONSE response; 
    response.error = CRC_SUCCESS;
    unsigned char buffer[9]; 
  
    int length = read(uartStream, buffer, 9);

    short crc = calcula_CRC(buffer, 7);
    memcpy(&response.crc, &buffer[7], 2);

    printf("CRC %d\n", crc); 
    printf("CRC ENVIADO %d\n", response.crc); 

    if(crc != response.crc){
        printf("Erro de CRC\n"); 
        response.error = CRC_ERROR; 
        return response; 
    }

    memcpy(&response.device_address, &buffer[0], 1); // copying code 
    memcpy(&response.modbus_code, &buffer[1], 1); // copying code 
    memcpy(&response.subcode, &buffer[2], 1); // copying code 
    memcpy(&response.data, &buffer[3], 4); // copying code 

    if(length < 0){
        printf("Erro na leitura read_int\n"); 
        return;
    }
    return response;
}





