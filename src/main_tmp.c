
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <irq.h>
#include <uart.h>
#include <i2c.h>

#include <math.h> 

#include <generated/csr.h>
#include <generated/soc.h>

#include "lidar.h"
#include "imu.h"
#include "camera.h"

int main(void){

    uart_rxtx_write(7); 

    uart_rxtx_write(131); 
    uart_rxtx_write(137); 
    
    
    
    return 0; 
}