#include "uart_functions.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "lcd.h"
#include "sensor.h"
#include "system_config.h"
#include <pthread.h>


void  init_system(); 
void shut_down_system(); 

int main(int argc, const char * argv[]) {
   init_system();

    // pthread_t sensor_thread_id;
    // pthread_create(&sensor_thread_id, NULL, (void*)read_room_temperature, NULL);

    menu();
    shut_down_system();
  
   return 0;
}


void shut_down_system(){
  SYSTEM_CONFIG config = get_current_config(); 
  closeUart(config.uart_stream);
}

void init_system(){
   int uart_stream = initUart("/dev/serial0");
    if(uart_stream == -1){
        exit(0); 
    }

    configUart(uart_stream);
    init_system_state(uart_stream);
    lcd_init();
    init_sensor();
}