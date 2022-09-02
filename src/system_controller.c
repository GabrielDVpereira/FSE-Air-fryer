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
        if(!is_time_over()) controll_system_temperature();
        if(!is_heating_system && !is_time_over()) start_system_timer(); 
        if(is_time_over()) stop_system(); 
 
    }
}


void controll_system_temperature(){
    printf("\n\n"); 
    SYSTEM_CONFIG config = get_current_config();

    MODBUS_RESPONSE response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    while(response_tf.error != CRC_SUCCESS || response_tf.subcode != REQUEST_REFERENCE_TEMPERATURE){
        response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }

    float tf; 
    memcpy(&tf, response_tf.data, 4);
    printf(" TR %f\n", tf); 

    MODBUS_RESPONSE response_ti = read_uart(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    while(response_ti.error != CRC_SUCCESS || response_ti.subcode != REQUEST_INTERNAL_TEMPERATURE){
        response_ti = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }

    float ti; 
    memcpy(&ti, response_ti.data, 4);
    printf(" TI %f\n", ti); 



    pid_atualiza_referencia(tf);
    double pid = pid_controle(ti); 

    printf("PID %lf \n", pid); 
    if(!is_time_over()) adjust_tempeture(pid);

    if(is_heating_system){
        show_message_lcd("Aquecendo..."); 
    } else {
        show_temperatute_lcd(ti, tf, config.time);
    }

    send_int_uart(config.uart_stream, (int)pid, SEND_CONTROL_SIGN);
     if(!is_time_over()) send_float_uart(config.uart_stream, tf, SEND_REFERENCE_SIGN);

    /* pid 0 means the system has reached the reference */ 
    if(ti >= (tf-1)){ // error margin of 1
        is_heating_system = 0;
    }
}

void start_system_timer(){
    printf("start_system_timer: %d", timer);
    timer++; 
    if(timer == 60){
        decrease_system_time(); 
        timer = 0; 
    }
}

void stop_system(){
    SYSTEM_CONFIG config = get_current_config();  

    MODBUS_RESPONSE response_ti = read_uart(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    while(response_ti.error != CRC_SUCCESS || response_ti.subcode != REQUEST_INTERNAL_TEMPERATURE){
        response_ti = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }
    float ti; 
    memcpy(&ti, response_ti.data, 4);

    float room_temp = read_room_temperature();
    printf(" TI %f  ---- ROOM TEMP %f\n", ti, room_temp); 

    if(ti > room_temp){
        show_message_lcd("Esfriando..."); 
        adjust_tempeture(-100);  // TURN ON FAN 
        send_float_uart(config.uart_stream, -100.0, SEND_REFERENCE_SIGN);
        return; 
    } 
    reset_controller_state(); 
}

void reset_controller_state(){
    printf("reset_controller_state\n");
    show_message_lcd("bye!"); 
    timer = 0; 
    is_heating_system = 1;
    adjust_tempeture(PWM_MIN);
    set_system_stoped(); 
}