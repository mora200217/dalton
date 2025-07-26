#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>
#include "delay.h"
#include "irobot2.h"

void loop(void);

void
loop(void) {
    roomba_motor_square_test();
    delay(7000);
}

int main(void){
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();

    roomba_reset();
    delay(500);
    roomba_start();
    delay(500);
    roomba_full_mode();
    while (1) loop();
	return 0;
}

