#include "generated/soc.h"
#include <stdio.h>
#include <stdlib.h>
#include <irq.h>
#include <uart.h>

#include <generated/csr.h>
#define PERIOD 1024 * 50  //

void my_wait(unsigned int ms)
{

    timer0_en_write(0);
    timer0_reload_read();
    timer0_load_write(CONFIG_CLOCK_FREQUENCY/1000*ms);
    timer0_en_write(1);
    timer0_update_value_write(1);
    while(timer0_value_read()) timer0_update_value_write(1);
}


int main(void)
{
    #ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);

#endif
    pwm_enable_write(1);
    pwm_period_write(PERIOD);

    uart_init();
    // puts("\n CPU testing cain_test\n");

    printf("Hola mundo!\n");
    while(1){
        printf("Iniciando conteo");
        my_wait(1000); // 1 segundo de espera

        for(int i = 0; i < 100; i++){
            pwm_width_write(i * 1024);
            my_wait(100);
        }
    }
    /* while(1){
        printf("Sí");
        leds_out_write(0);
        my_wait(250);
        leds_out_write(1);
        my_wait(250);
    }*/
}
