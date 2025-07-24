#include <time.h>
#include "delay.h"

// https://electronics.stackexchange.com/questions/450971/embedded-c-most-elegant-way-to-insert-a-delay
// time ~ 5*T clock ticks

#pragma GCC push_options
#pragma GCC optimize ("O0")
void delay (int milliseconds) {
    unsigned int end_T = milliseconds/5 * CLOCKS_PER_SEC/1000;
    for(unsigned int i=0; i<end_T; i++){
        __asm__("addi x0, x0, 0");
    }
#pragma GCC pop_options
}

