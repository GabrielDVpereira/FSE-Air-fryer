#include "menu.h"
#include <stdio.h>
#include "uart_functions.h"
#include <unistd.h>
#include "uart_config.h"
#include "pid.h"
#include <string.h>
#include "lcd.h"
#include "system_config.h"


void handle_turn_on_oven();
void handle_turn_off_oven();
void handle_add_time();
void handle_decrease_time();
void handle_running();
void handle_stoped();

void menu(){
    int time = 0; 

    SYSTEM_CONFIG config = get_current_config(); 

    // while (1)
    // {   
    //     time++;
    //     pid_configura_constantes(30.0, 0.2, 400.0);
    //     float ti = readFloat(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    //     float tf = readFloat(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    //     printf(" TI %f\n", ti);  
    //     printf(" TR %f\n", tf);  
    //     pid_atualiza_referencia(tf);
    //     printf("PID %f\n", pid_controle(ti)); 
    //     displayTempetureValues(ti, tf, time);
    //     usleep(500000);    
    // }

    while (1)
    {   
        int userInput = readInt(config.uart_stream, REQUEST_USER_INPUTS); 
        switch (userInput)
        {
            case USER_INPUT_TURN_ON_OVEN:
                handle_turn_on_oven(); 
                break;
                
            case USER_INPUT_TURN_OFF_OVEN:
                handle_turn_off_oven(); 
                break;

            case USER_INPUT_ADD_TIME: 
                handle_add_time(); 
                break;

            case USER_INPUT_DECREASE_TIME: 
                handle_decrease_time(); 
                break; 

            case USER_INPUT_START_OVEN: 
                handle_running(); 
                break; 

            case USER_INPUT_CANCEL_OVEN: 
                handle_stoped(); 
                break; 

            default:
                break;
        }
        userInput = 0; 
        usleep(50000); 
    }
}

void handle_turn_on_oven(){
    printf("handle_turn_on_oven\n"); 
    set_system_state_on();
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.system_state: %d\n",  config.system_state);
    sendByteUart(config.uart_stream,  config.system_state,  SEND_SISTEM_STATE);
}

void handle_turn_off_oven(){
    printf("handle_turn_off_oven\n"); 
    set_system_state_off();
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.system_state: %d\n",  config.system_state);
    sendByteUart(config.uart_stream,  config.system_state,  SEND_SISTEM_STATE);
}

void handle_add_time(){

    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }

    printf("handle_add_time\n"); 
    add_system_time(); 
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.time %d\n", config.time); 
    sendIntUart(config.uart_stream,  config.time,  SEND_SYSTEM_TIME);
}

void handle_decrease_time(){

    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }
    printf("handle_decrease_time\n"); 
    decrease_system_time(); 
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.time %d\n", config.time); 
    sendIntUart(config.uart_stream,  config.time,  SEND_SYSTEM_TIME);
}


void handle_running(){
    printf("handle_running\n"); 
    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }
    set_system_running(); 
    SYSTEM_CONFIG config = get_current_config(); 
    sendByteUart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);
}

void handle_stoped(){
    printf("handle_stoped\n"); 
    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }
    set_system_stoped(); 
    SYSTEM_CONFIG config = get_current_config(); 
    sendByteUart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);
}
    
    // while (1)
    // {   
    //     time++;
    //     pid_configura_constantes(30.0, 0.2, 400.0);
    //     float ti = readTemperature(uartStream, REQUEST_INTERNAL_TEMPERATURE);
    //     float tf = readTemperature(uartStream, REQUEST_REFERENCE_TEMPERATURE);
    //     printf(" TI %f\n", ti);  
    //     printf(" TR %f\n", tf);  
    //     pid_atualiza_referencia(tf);
    //     printf("PID %f\n", pid_controle(ti)); 
    //     displayTempetureValues(ti, tf, time);
    //     sleep(1);    
    // }
    


    // char cmd[] = { SEND_INT,SEND_FLOAT, SEND_STRING,REQUEST_INT, REQUEST_FLOAT, REQUEST_STRING };
    // int option; 
    
    // printf("Digite uma opção:\n");  
    // printf("0 - Enviar inteiro\n");  
    // printf("1 - Enviar fload\n");  
    // printf("2 - Enviar string\n");  
    // printf("3 - ler inteiro\n");  
    // printf("4 - ler float\n");  
    // printf("5 - ler string\n");  
    
    // while(scanf("%d", &option) == 1){
    //     if(cmd[option] == SEND_INT) return sendIntMenu(uartStream); 
    //     if(cmd[option] == SEND_FLOAT) return sendFloatMenu(uartStream); 
    //     if(cmd[option] == SEND_STRING) return sendStringMenu(uartStream); 
    //     if(cmd[option] == REQUEST_INT) return readInd(uartStream); 
    //     if(cmd[option] == REQUEST_FLOAT) return readFloat(uartStream); 
    //     if(cmd[option] == REQUEST_STRING) return readString(uartStream); 
    // }


// void sendIntMenu(int uartStream){
//     int data; 
//     printf("Digite o inteiro que deseja enviar: "); 
//     scanf("%d", &data); 
//     sendIntUart(uartStream, data); 
// }

// void sendFloatMenu(int uartStream){
//     float data; 
//     printf("Digite o float que deseja enviar: "); 
//     scanf("%f", &data); 
//     sendFloatUart(uartStream, data); 
// }

// void sendStringMenu(int uartStream){
//     sendStringUart(uartStream, "GABS"); 
// }