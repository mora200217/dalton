#ifndef _FPGA_IO
#define _FPGA_IO

#include "irq.h"

static inline void fpgaio_enable_interrupts(void) {
#ifdef CONFIG_CPU_HAS_INTERRUPT
    irq_setmask(0);
    irq_setie(1);
#endif
}

#endif