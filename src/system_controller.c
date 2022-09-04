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
#include "uart_functions.h"
#include "modbus.h"
#include <string.h>
#include <unistd.h>         //Used for UART
#include <pthread.h>
#include "log.h"



void  controll_system_temperature();
void  start_system_timer(); 
void  stop_system(); 
void reset_controller_state();
int is_idle_temperature();

int started_timer = 0; 
int is_adjusting_temperature = 1; 

double pwm; 

void control_temperature(){ 
    while (is_system_running()){  
        if(should_kill_sytem()){
            reset_controller_state(); 
            return;
        }
        if(!is_time_over()) controll_system_temperature();
        if(!is_adjusting_temperature && !started_timer) start_system_timer(); 
        if(is_time_over()) stop_system(); 
        sleep(1);
    }

    started_timer = 0; 
    is_adjusting_temperature = 1; 
}

void control_timer(){
    SYSTEM_CONFIG config = get_current_config();
    DEFINED_MODE mode = get_mode(); 
    int timer = config.time * 60; 
    while(!is_time_over() && is_system_running()){
        SYSTEM_TEMP temp = get_current_temp(); 
        if(is_menu_mode_on()) {
            show_temperatute_lcd_mode(temp.ti, temp.tr, timer,mode.name); 
        } else {
            show_temperatute_time(temp.ti, temp.tr, timer);
        }
        timer--; 
        if(timer % 60  == 0){
            decrease_system_time(); 
        }
        writte_csv_log(temp.ti,temp.room_temp,temp.tr,(int)pwm);
        sleep(1); 
    }
    started_timer = 0; 
}


void controll_system_temperature(){
    SYSTEM_TEMP temp = get_current_temp(); 
    pid_atualiza_referencia(temp.tr);
    pwm = pid_controle(temp.ti); 

    adjust_tempeture(pwm);

    if(is_adjusting_temperature){
        show_temperatute_lcd_adjusting(temp.ti, temp.tr);
    } 

    SYSTEM_CONFIG config = get_current_config();
    send_int_uart(config.uart_stream, (int)pwm, SEND_CONTROL_SIGN);
    send_float_uart(config.uart_stream, temp.tr, SEND_REFERENCE_SIGN);

    if(is_idle_temperature()){ 
        is_adjusting_temperature = 0;
    }
}

int is_idle_temperature(){
    SYSTEM_TEMP temp = get_current_temp(); 
    float ERROR_MARGIN = 0.5; 
    return temp.ti >= (temp.tr - ERROR_MARGIN) && temp.ti <= (temp.tr + ERROR_MARGIN); // error margin of 1
}

void start_system_timer(){
    started_timer = 1; 
    pthread_t timer_t_id;
    pthread_create(&timer_t_id, NULL, (void*)control_timer, NULL);
}

void stop_system(){
    SYSTEM_CONFIG config = get_current_config();  
    SYSTEM_TEMP temp = get_current_temp(); 

    if(temp.ti > temp.room_temp){
        show_temperatute_lcd_cooling(temp.ti, temp.room_temp); 
        adjust_tempeture(-100);  // TURN ON FAN 
        send_float_uart(config.uart_stream, -100, SEND_CONTROL_SIGN);
        return; 
    } 
    reset_controller_state(); 
}

void reset_controller_state(){
    printf("reset_controller_state\n");
    show_message_lcd("Finalidado!"); 
    is_adjusting_temperature = 1;
    adjust_tempeture(PWM_MIN);
    
    // Make system stop 
    set_system_stoped(); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_byte_uart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);
    send_int_uart(config.uart_stream, 0, SEND_SYSTEM_TIME);
    if(is_menu_mode_on()) set_menu_mode_off();
}