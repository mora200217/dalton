
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <uart.h>
#include <i2c.h>

#include <math.h> 

#include <generated/csr.h>
#include <generated/soc.h>

#include"fpgaio.h"
#include "utils.h"
#include "lidar.h"



int main(void){
    fpgaio_enable_interrupts(); 
    uart_init();

    printf("Welcome to Dalton! \n"); 
    printf("Testing..."); 

       while (1) {
       my_wait(1000); 

      

       printf("...\n"); 
        if (recibir_paquete()) {
            procesar_distancia();
            manejar_estado_por_distancia();
        }
    }

    return 0; 
}