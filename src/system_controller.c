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




void  controll_system_temperature();
void  start_system_timer(); 
void  stop_system(); 
void reset_controller_state();

int started_timer = 0; 
int is_heating_system = 1; 
float tf; 
float ti; 

void control_temperature(){ 
    printf(" INITIALIZE TEMPERATURE CONTROL %d\n", is_system_running());
    while (is_system_running()){   
        if(should_kill_sytem()){
            reset_controller_state(); 
            return;
        }
        if(!is_time_over()) controll_system_temperature();
        if(!is_heating_system && !started_timer) start_system_timer(); 
        if(is_time_over()) stop_system(); 
 
    }
}

void control_timer(){
    SYSTEM_CONFIG config = get_current_config();
    int timer = config.time * 60; 
    while(!is_time_over()){
        show_temperatute_lcd(ti, tf, timer);
        timer--; 
        if(timer % 60  == 0){
            decrease_system_time(); 
        }
        sleep(1); 
    }
    started_timer = 0; 
}


void controll_system_temperature(){
    printf("controll_system_temperature\n"); 
    printf("\n\n"); 
    SYSTEM_CONFIG config = get_current_config();

    MODBUS_RESPONSE response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    while(response_tf.error != CRC_SUCCESS || response_tf.subcode != REQUEST_REFERENCE_TEMPERATURE){
        printf("Error TR %d %x\n",response_tf.error, response_tf.subcode );
        response_tf = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }

    memcpy(&tf, response_tf.data, 4);
    printf(" TR %f\n", tf); 

    MODBUS_RESPONSE response_ti = read_uart(config.uart_stream, REQUEST_INTERNAL_TEMPERATURE);
    while(response_ti.error != CRC_SUCCESS || response_ti.subcode != REQUEST_INTERNAL_TEMPERATURE){
        printf("Error TI\n");
        response_ti = read_uart(config.uart_stream, REQUEST_REFERENCE_TEMPERATURE);
    }
    printf("controll_system_temperature 2\n"); 
  
    memcpy(&ti, response_ti.data, 4);
    printf(" TI %f\n", ti); 



    pid_atualiza_referencia(tf);
    double pid = pid_controle(ti); 

    printf("PID %lf \n", pid); 
    adjust_tempeture(pid);

    if(is_heating_system){
        show_message_lcd("Aquecendo..."); 
    } 

    send_int_uart(config.uart_stream, (int)pid, SEND_CONTROL_SIGN);
     if(!is_time_over()) send_float_uart(config.uart_stream, tf, SEND_REFERENCE_SIGN);

    if(ti >= (tf-1)){ // error margin of 1
        is_heating_system = 0;
    }
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
    is_heating_system = 1;
    adjust_tempeture(PWM_MIN);
    
    // Make system stop 
    set_system_stoped(); 
    SYSTEM_CONFIG config = get_current_config(); 
    send_byte_uart(config.uart_stream,  config.system_running,  SEND_SYSTEM_RUNNING);
}