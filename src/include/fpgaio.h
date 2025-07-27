   

   #ifndef _FPGA_IO
  #define _FPGA_IO 
   
  
  #include "irq.h" 
  
    #ifdef CONFIG_CPU_HAS_INTERRUPT
    
        irq_setmask(0);
        irq_setie(1);
        #endif


    #endif 