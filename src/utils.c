#include <stdio.h>
#include "utils.h"

void printBuffer(unsigned char * buffer) {
    for(int i=0; i < 9; i++){
    printf("%x", buffer[i]);
    }
    printf("\n");
}
