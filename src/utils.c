#include <stdio.h>
#include "utils.h"

void print_buffer(unsigned char * buffer) {
    for(int i=0; i < 9; i++){
    printf("%x", buffer[i]);
    }
    printf("\n");
}
