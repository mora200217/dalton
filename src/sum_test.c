#include "generated/soc.h"
#include <stdio.h>
#include <stdlib.h>
#include <irq.h>
#include <uart.h>
#include <generated/csr.h>
#define PERIOD 1024 * 50  //



int main(void)
{
    #ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);

#endif

    uart_init();
    // puts("\n CPU testing cain_test\n");

    printf("Hola mundo!\n");
    printf("Estoy en SUM.v!\n");

    sum_ev_enable_read();
    sum_ev_enable_write(32);



    for(int i = 0; i < 2; i ++){
        for(int j = 0 ; j < 2; j++){
            sum_a_write(i);
            sum_b_write(j);

             int c  = sum_c_read();

            printf("a: %d b: %d  => c: %d \n", i, j, c);
        }
    }


    while(1){

    }

}
