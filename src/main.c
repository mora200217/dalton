
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include <uart.h>
#include <i2c.h>

#include <math.h> 

#include <generated/csr.h>
#include <generated/soc.h>

#include"fpgaio.h"

int main(void){
    fpgaio_enable_interrupts(); 
    uart_init();

    printf("Welcome to Dalton!"); 

    // recibir_paquete();  
    return 0; 
}