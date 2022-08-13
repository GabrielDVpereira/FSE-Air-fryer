#include "menu.h"
#include <stdio.h>
#include "uart_functions.h"
#include "config.h"

void menu(int uartStream){
    char cmd[] = { SEND_INT,SEND_FLOAT, SEND_STRING,REQUEST_INT, REQUEST_FLOAT, REQUEST_STRING };
    int option; 
    
    printf("Digite uma opção:\n");  
    printf("0 - Enviar inteiro\n");  
    printf("1 - Enviar fload\n");  
    printf("2 - Enviar string\n");  
    printf("3 - ler inteiro\n");  
    printf("4 - ler float\n");  
    printf("5 - ler string\n");  
    
    while(scanf("%d", &option) == 1){
        if(cmd[option] == SEND_INT) return sendIntMenu(uartStream); 
        if(cmd[option] == SEND_FLOAT) return sendFloatMenu(uartStream); 
        if(cmd[option] == SEND_STRING) return sendStringMenu(uartStream); 
        if(cmd[option] == REQUEST_INT) return readInd(uartStream); 
        if(cmd[option] == REQUEST_FLOAT) return readFloat(uartStream); 
        if(cmd[option] == REQUEST_STRING) return readString(uartStream); 
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

void sendStringMenu(int uartStream){
    char data[255]; 
    // printf("Digite a string que deseja enviar: "); 
    // fgets(data, sizeof(data), stdin); 
    sendStringUart(uartStream, "TESTE STRING"); 
}