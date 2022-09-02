#include <stdio.h>
#include "system_config.h"


SYSTEM_CONFIG config; 

SYSTEM_CONFIG get_current_config(){
    return config;
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
    config.system_state = SYSTEM_OFF; 
}

void set_system_running(){
    config.system_running = SYSTEM_RUNING;
}

void set_system_stoped(){
    config.system_running = SYSTEM_STOPED;
}

void set_kill_system(){
    config.kill_all = KILL_SYSTEM;
}
