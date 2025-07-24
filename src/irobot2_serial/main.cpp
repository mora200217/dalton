#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>
#include "delay.h"
#include "irobot2.h"

void loop(void);

iRobotCreate2 roomba;

void
loop(void) {
    static char done = 0;
    if (done) return;
    roomba.motorSquareTest();
    delay(7000);
    done = 1;
    roomba.passiveMode();
}

int main(void){
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();

    iRobotCreate2 roomba;
    roomba.start();

    while (1) loop();
	return 0;
}

