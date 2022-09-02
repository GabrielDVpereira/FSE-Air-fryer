#include <stdio.h>
#include <unistd.h>
#include "uart_functions.h"
#include "system_controller.h"
#include "system_config.h"
#include "uart_config.h"
#include "lcd.h"
#include "pid.h"
#include "gpio.h"
#include "sensor.h"


void  controll_system_temperature();
void  start_system_timer(); 
void  stop_system(); 
void reset_controller_state();

int timer = 0; 
int is_heating_system = 1; 

void control_temperature(){ 
    while (is_system_running()){   

        if(should_kill_sytem()){
            reset_controller_state(); 
            return;
        }

        controll_system_temperature();
        if(!is_heating_system) start_system_timer(); 
        if(is_time_over()) stop_system(); 
        sleep(1); 
 
    }
}


void controll_system_temperature(){
    SYSTEM_CONFIG config = get_current_config();  
    float ti = read_float(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    float tf = read_float(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);

    printf(" TI %f\n", ti);  
    printf(" TR %f\n", tf); 

    pid_atualiza_referencia(tf);
    double pid = pid_controle(ti); 

    printf("PID %lf\n", pid); 
    adjust_tempeture(pid); 
    show_temperatute_lcd(ti, tf, config.time);

    send_int_uart(config.uart_stream, (int)pid, SEND_CONTROL_SIGN);
    send_float_uart(config.uart_stream, tf, SEND_REFERENCE_SIGN);

    /* pid 0 means the system has reached the reference */ 
    if(pid == 0){
        is_heating_system = 0;
    }
}

void start_system_timer(){
    timer++; 
    if(timer == 60){
        decrease_system_time(); 
        timer = 0; 
    }
}

void stop_system(){
    SYSTEM_CONFIG config = get_current_config();  

    float ti = read_float(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    float room_temp = read_room_temperature();

    if(ti > room_temp){
        adjust_tempeture(-100);  // TURN ON FAN 
        return; 
    } 
    reset_controller_state(); 
}

void reset_controller_state(){
    timer = 0; 
    is_heating_system = 1;
    adjust_tempeture(PWM_MIN);
    set_system_stoped(); 
}