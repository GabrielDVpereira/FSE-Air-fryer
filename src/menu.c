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
void handle_menu_mode();
void handle_user_input(int); 


void menu(){
    SYSTEM_CONFIG config = get_current_config();    

    while (1)
    {   
        if(should_kill_sytem()) return; 
        if(is_system_running()) continue;
        
        MODBUS_RESPONSE response = read_uart(config.uart_stream, REQUEST_USER_INPUTS); 
        
        int user_input; 
        memcpy(&user_input, response.data, 4);

        if(response.subcode != REQUEST_USER_INPUTS) continue; 
        if(response.error != CRC_SUCCESS) continue; 

        handle_user_input(user_input);        
        usleep(50000); 
    }
}

void handle_user_input(int option){
    switch (option){
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

            case USER_INPUT_MENU_MODE: 
                handle_menu_mode(); 
                break; 
            default:
                break;
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

    if(is_system_running()){
        printf("System is Already Running\n"); 
        return;
    }

    set_system_running(); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_byte_uart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);

    pthread_t controller_thread_id;
    pthread_create(&controller_thread_id, NULL, (void*)control_temperature, NULL);
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

void handle_menu_mode(){
    if(!is_system_on()){
        printf("You must turn on the System\n"); 
        return;
    }

    show_message_lcd("Modo Menu"); 
    DEFINED_MODE chicken = { .time = 3, .tr = 40.0, .name = "Frango" }; 
    DEFINED_MODE meat = { .time = 5, .tr = 50.0, .name = "Carne"}; 
    DEFINED_MODE nuggets = { .time = 1, .tr = 30.0, .name = "Nugget"}; 

    DEFINED_MODE options[] = { chicken, meat, nuggets }; 
    int option; 
    printf("Digite uma opção:\n");  
    printf("1 - Assar frango\n");  
    printf("2 - Descongelar Carne\n");  
    printf("3 - Fritar Nuggets\n");  
    printf("4 - Sair\n");  
    scanf("%d", &option); 

    if(option == 4) return; 

    while(option < 1 || option > 4){
        printf("Digite uma opção válida:  \n");
        scanf("%d", &option); 
    }
    DEFINED_MODE selected_option = options[option - 1]; 
    set_menu_mode_on();
    set_new_time(selected_option.time); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_float_uart(config.uart_stream, selected_option.tr, SEND_REFERENCE_SIGN);
    send_int_uart(config.uart_stream, selected_option.time, SEND_SYSTEM_TIME);
    set_mode(selected_option); 
    char msg[20] = "Modo: "; 
    strcat(msg, selected_option.name); 
    show_message_lcd(msg); 
    sleep(1);
    handle_running();

}

