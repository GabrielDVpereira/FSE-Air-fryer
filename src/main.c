#include "uart_functions.h"
#include "menu.c"
#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART

int main(int argc, const char * argv[]) {
    menu(1);

    // int uart_stream = initUart("/dev/serial0");
    
    // if(uart_stream == -1){
    //     exit(0); 
    // }

    // configUart(uart_stream);
    // menu(uart_stream);
    // closeUart(uart_stream);

  
   return 0;
}
