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
void update_tr();
void update_ti();
int is_idle_temperature();
void update_room_temp();
void update_temp(); 



int started_timer = 0; 
int is_adjusting_temperature = 1; 
float tr; 
float ti; 
double pwm; 
float room_temp; 

void control_temperature(){ 
    printf(" INITIALIZE TEMPERATURE CONTROL %d\n", is_system_running());
    while (is_system_running()){  
        update_temp(); 
        if(should_kill_sytem()){
            reset_controller_state(); 
            return;
        }
        if(!is_time_over()) controll_system_temperature();
        if(!is_adjusting_temperature && !started_timer) start_system_timer(); 
        if(is_time_over()) stop_system(); 
    }
}

void control_timer(){
    SYSTEM_CONFIG config = get_current_config();
    DEFINED_MODE mode = get_mode(); 

    int timer = config.time * 60; 
    while(!is_time_over()){
        if(is_menu_mode_on()) {
            show_temperatute_lcd_mode(ti, tr, timer,mode.name); 
        } else {
            show_temperatute_time(ti, tr, timer);
        }
        timer--; 
        if(timer % 60  == 0){
            decrease_system_time(); 
        }
        writte_csv_log(ti,room_temp,tr,(int)pwm);
        sleep(1); 
    }
    started_timer = 0; 
}


void controll_system_temperature(){
    printf("controll_system_temperature\n"); 
    printf("\n\n"); 

    pid_atualiza_referencia(tr);
    pwm = pid_controle(ti); 

    printf("PID %lf \n", pwm); 
    adjust_tempeture(pwm);

    if(is_adjusting_temperature){
        show_temperatute_lcd_adjusting(ti, tr);
    } 

    SYSTEM_CONFIG config = get_current_config();
    send_int_uart(config.uart_stream, (int)pwm, SEND_CONTROL_SIGN);
    send_float_uart(config.uart_stream, tr, SEND_REFERENCE_SIGN);

    if(is_idle_temperature()){ 
        printf("is_idle_temperature\n"); 
        is_adjusting_temperature = 0;
    }
}

int is_idle_temperature(){
    float ERROR_MARGIN = 0.5; 
    return ti >= (tr - ERROR_MARGIN) && ti <= (tr + ERROR_MARGIN); // error margin of 1
}

void update_temp(){
    update_tr(); 
    update_ti(); 
    update_room_temp(); 
}


void update_tr(){
    if(is_menu_mode_on()){
        DEFINED_MODE mode = get_mode();
        tr = mode.tr; 
        printf(" TR %f\n", tr); 
        return; 
    }

    SYSTEM_CONFIG config = get_current_config();
    MODBUS_RESPONSE response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    while(response_tf.error != CRC_SUCCESS || response_tf.subcode != REQUEST_REFERENCE_TEMPERATURE){
        printf("Error TR %d %x\n",response_tf.error, response_tf.subcode );
        response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }

    memcpy(&tr, response_tf.data, 4);
    printf(" TR %f\n", tr); 
}

void update_ti(){
    SYSTEM_CONFIG config = get_current_config();
    MODBUS_RESPONSE response_ti = read_uart(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    while(response_ti.error != CRC_SUCCESS || response_ti.subcode != REQUEST_INTERNAL_TEMPERATURE){
        printf("Error TI\n");
        response_ti = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }
  
    memcpy(&ti, response_ti.data, 4);
    printf(" TI %f\n", ti); 
}

void update_room_temp(){
  room_temp = read_room_temperature();
}

void start_system_timer(){
    started_timer = 1; 
    pthread_t timer_t_id;
    pthread_create(&timer_t_id, NULL, (void*)control_timer, NULL);
}

void stop_system(){
    SYSTEM_CONFIG config = get_current_config();  

    MODBUS_RESPONSE response_ti = read_uart(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    while(response_ti.error != CRC_SUCCESS || response_ti.subcode != REQUEST_INTERNAL_TEMPERATURE){
        response_ti = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }

    memcpy(&ti, response_ti.data, 4);

    printf(" TI %f  ---- ROOM TEMP %f\n", ti, room_temp); 

    if(ti > room_temp){
        show_temperatute_lcd_cooling(ti, room_temp); 
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