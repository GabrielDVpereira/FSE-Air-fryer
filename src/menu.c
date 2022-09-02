#include "menu.h"
#include <stdio.h>
#include "uart_functions.h"
#include <unistd.h>
#include "uart_config.h"
#include "pid.h"
#include <string.h>
#include "lcd.h"
#include "system_config.h"
#include <pthread.h>
#include "system_controller.h"
#include "modbus.h"

void handle_turn_on_oven();
void handle_turn_off_oven();
void handle_add_time();
void handle_decrease_time();
void handle_running();
void handle_stoped();


void menu(){
    SYSTEM_CONFIG config = get_current_config();    

    while (1)
    {   
        if(should_kill_sytem()) return; 

        MODBUS_RESPONSE response = read_uart(config.uart_stream, REQUEST_USER_INPUTS); 
        
        int user_input; 
        memcpy(&user_input, response.data, 4);

        if(response.subcode != REQUEST_USER_INPUTS) continue; 
        if(response.error != CRC_SUCCESS) continue; 

        printf("USER INPUT: %d\n", user_input);
        switch (user_input)
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
        usleep(50000); 
    }
}

void handle_turn_on_oven(){
    printf("handle_turn_on_oven\n"); 
    set_system_state_on();
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.system_state: %d\n",  config.system_state);
    send_byte_uart(config.uart_stream,  config.system_state,  SEND_SISTEM_STATE);
}

void handle_turn_off_oven(){
    printf("handle_turn_off_oven\n"); 
    set_system_state_off();
    SYSTEM_CONFIG config = get_current_config(); 
    printf("config.system_state: %d\n",  config.system_state);
    send_byte_uart(config.uart_stream,  config.system_state,  SEND_SISTEM_STATE);
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
    send_int_uart(config.uart_stream,  config.time,  SEND_SYSTEM_TIME);
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
    send_int_uart(config.uart_stream,  config.time,  SEND_SYSTEM_TIME);
}


void handle_running(){
    printf("handle_running\n"); 
    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }
    set_system_running(); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_byte_uart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);

    // pthread_t controller_thread_id;
    // pthread_create(&controller_thread_id, NULL, (void*)control_temperature, NULL);
}

void handle_stoped(){
    printf("handle_stoped\n"); 
    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }
    set_system_stoped(); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_byte_uart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);
}
