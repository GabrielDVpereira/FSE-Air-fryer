#include <stdio.h>
#include "system_config.h"


SYSTEM_CONFIG config; 
DEFINED_MODE mode; 

SYSTEM_CONFIG get_current_config(){
    return config;
}

DEFINED_MODE get_mode(){
    return mode; 
}

void  set_mode(DEFINED_MODE new_mode){
    mode = new_mode; 
}

int is_menu_mode_on(){
    return  config.menu_mode == MENU_MODE_ON; 
}

void set_menu_mode_on(){
    config.menu_mode = MENU_MODE_ON; 
}

void set_menu_mode_off(){
    config.menu_mode = MENU_MODE_OFF; 
}

int is_system_on(){
    return config.system_state == SYSTEM_ON; 
}

int is_system_running(){
    return config.system_running == SYSTEM_RUNING; 
}

int is_time_over(){
    return config.time == 0; 
}

void init_system_state(int uart_stream){
   config.uart_stream = uart_stream; 
   config.time = 0;
   config.system_state = SYSTEM_OFF; 
   config.system_running = SYSTEM_STOPED; 
   config.kill_all = SYSTEM_IDLE; 
}

int should_kill_sytem(){
    return config.kill_all == KILL_SYSTEM; 
}

void set_new_time(int time){
    config.time = time; 
}

void add_system_time(){
    config.time+=1;
}

void decrease_system_time(){
    if(config.time > 0){
        config.time-=1;
    }
}

void set_system_state_on(){
    config.system_state = SYSTEM_ON; 
}

void set_system_state_off(){
   config.time = 0;
   config.system_state = SYSTEM_OFF; 
   config.system_running = SYSTEM_STOPED; 
   config.kill_all = SYSTEM_IDLE; 
   config.kill_all = KILL_SYSTEM;
}

void set_system_running(){
    config.system_running = SYSTEM_RUNING;
}

void set_system_stoped(){
    config.system_running = SYSTEM_STOPED;
}

void set_kill_system(){
    config.kill_all = KILL_SYSTEM;
    config.system_running = SYSTEM_STOPED;
    config.system_state = SYSTEM_OFF; 
}
