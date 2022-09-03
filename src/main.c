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
#include "gpio.h"
#include "pid.h"
#include<signal.h>
#include "system_controller.h"
#include "uart_config.h"
#include "lcd.h"
#include "log.h"

#define SIGINT  2   /* Interrupt the process */ 


void init_system(); 
void shut_down_system(); 
void update_board();

void handle_sigint(int sig)
{
  shut_down_system();
}


int main(int argc, const char * argv[]) {
  signal(SIGINT, handle_sigint);
  init_system();
  menu();
  shut_down_system();
   return 0;
}


void shut_down_system(){
  show_message_lcd("Desligando...");
  sleep(1); 
  show_message_lcd("");
  set_kill_system(); 
  SYSTEM_CONFIG config = get_current_config(); 
  update_board();
  sleep(1); 
  close_uart(config.uart_stream);
  close_csv_file();
}

void init_system(){
  int uart_stream = init_uart("/dev/serial0");
  if(uart_stream == -1) exit(0); 
  pid_configura_constantes(30.0, 0.2, 400.0);
  config_uart(uart_stream);
  init_system_state(uart_stream);
  lcd_init();
  init_sensor();
  initialize_gpio();
  update_board();
  init_csv_log();
}

void update_board(){
  SYSTEM_CONFIG config = get_current_config(); 
  send_byte_uart(config.uart_stream, config.system_state, SEND_SISTEM_STATE);
  usleep(500000); 
  send_byte_uart(config.uart_stream, config.system_running, SEND_SYSTEM_RUNNING);
  usleep(500000); 
  send_int_uart(config.uart_stream, config.time, SEND_SYSTEM_TIME);
}