

#include <generated/csr.h>
#include <generated/soc.h>

#include "utils.h"

void my_wait(unsigned int ms)
{
    timer0_en_write(0);
    timer0_reload_read();
    timer0_load_write(CONFIG_CLOCK_FREQUENCY/1000*ms);
    timer0_en_write(1);
    timer0_update_value_write(1);
    while (timer0_value_read()) timer0_update_value_write(1);
}