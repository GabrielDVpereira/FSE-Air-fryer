#include <stdio.h>
#include "config.c"
#include "uart_functions.h"

void sendIntMenu(int);
void sendFloatMenu(int);
void sendStringMenu(int);

void menu(int uartStream){
    int cmd[] = { SEND_INT,SEND_FLOAT, SEND_STRING,REQUEST_INT, REQUEST_FLOAT, REQUEST_STRING };
    int option; 
    
    printf("Digite uma opção:\n");  
    printf("0 - Enviar inteiro\n");  
    printf("1 - Enviar fload\n");  
    printf("2 - Enviar string\n");  
    printf("3 - ler inteiro\n");  
    printf("4 - ler float\n");  
    printf("5 - ler string\n");  
    
    while(scanf("%d", &option) == 1){
        switch (cmd[option])
        {
        case SEND_INT:
            sendIntMenu(uartStream); 
            return; 
        case SEND_FLOAT: 
            sendFloatMenu(uartStream); 
            return; 
        case SEND_STRING: 
            sendStringMenu(uartStream); 
            return; 
        case REQUEST_INT: 
            readInd(uartStream); 
            return; 
        case REQUEST_FLOAT: 
            readFloat(uartStream); 
            return; 
        case REQUEST_STRING: 
            readString(uartStream); 
            return; 
        default:
            break;
        }
    }

}


void sendIntMenu(int uartStream){
    int data; 
    printf("Digite o inteiro que deseja enviar: "); 
    scanf("%d", &data); 
    sendIntUart(uartStream, data); 
}

void sendFloatMenu(int uartStream){
    float data; 
    printf("Digite o float que deseja enviar: "); 
    scanf("%f", &data); 
    sendFloatUart(uartStream, data); 
}

void sendStringMenu(uartStream){
    char data[255]; 
    printf("Digite a string que deseja enviar: "); 
    fgets(data, sizeof(data), stdin); 
    sendStringUart(uartStream, data); 
}