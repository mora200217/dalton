
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include <uart.h>
#include <i2c.h>

#include <math.h> 

#include <generated/csr.h>
#include <generated/soc.h>

#include"fpgaio.h"




// #include "lidar.h"
// #include "include/lidar.h"
// #include "imu.h"
// #include "camera.h"

int main(void){
    
 

       uart_init();

       printf("Welcome to Dalton!"); 

    // recibir_paquete();  
    return 0; 
}