#include "uart_functions.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART

int main(int argc, const char * argv[]) {
    int uart_stream = initUart("/dev/serial0");
    
    if(uart_stream == -1){
        exit(0); 
    }

    configUart(uart_stream);
    printf("configUart");
    menu(uart_stream);
    printf("After menu");
    closeUart(uart_stream);

  
   return 0;
}
