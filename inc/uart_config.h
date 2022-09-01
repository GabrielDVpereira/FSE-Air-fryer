#ifndef UART_CONFIG
#define UART_CONFIG

// DEVICE ADDRESS

#define DEVICE_ADDRESS 0x01

// REQUEST DATA CODE

#define REQUEST_DATA 0x23

// SEND DATA 

#define SEND_DATA 0x16

// REQUEST DADA SUB-CODES

#define REQUEST_INTERNAL_TEMPERATURE 0xc1
#define REQUEST_REFERENCE_TEMPERATURE 0xc2
#define REQUEST_USER_INPUTS 0xc3

// SEND DADA SUB-CODES


#define SEND_CONTROL_SIGN 0xD1 
#define SEND_REFERENCE_SIGN 0xD2
#define SEND_SISTEM_STATE 0xD3
#define SEND_SYSTEM_RUNNING 0xD5
#define SEND_SYSTEM_TIME 0xD6

// RESPONSES UART

#define USER_INPUT_TURN_ON_OVEN 0x1
#define USER_INPUT_TURN_OFF_OVEN 0x2
#define USER_INPUT_START_OVEN 0x3
#define USER_INPUT_CANCEL_OVEN 0x4
#define USER_INPUT_ADD_TIME 0x05
#define USER_INPUT_DECREASE_TIME 0x06
#define USER_INPUT_MENU_MODE 0x07



#endif