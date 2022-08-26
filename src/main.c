#include "uart_functions.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "lcd.h"

int main(int argc, const char * argv[]) {
    int uart_stream = initUart("/dev/serial0");
    
    if(uart_stream == -1){
        exit(0); 
    }
    lcd_init();
    configUart(uart_stream);
    menu(uart_stream);
    closeUart(uart_stream);

  
   return 0;
}
