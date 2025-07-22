#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include "include/bmi_sensor.h"

#include <irq.h>
#include <uart.h>
#include <i2c.h>

#include <generated/csr.h>
#include <generated/soc.h>

void my_wait(unsigned int ms)
{
    timer0_en_write(0);
    timer0_reload_read();
    timer0_load_write(CONFIG_CLOCK_FREQUENCY/1000*ms);
    timer0_en_write(1);
    timer0_update_value_write(1);
    while (timer0_value_read()) timer0_update_value_write(1);
}

/**
 * init_bmi
 * --
 * Envia 0x11 al registro 0x76 como comando de activacion para
 * los registros de lectura del sensor
 *
 *
 * */
int init_bmi(){
    unsigned char data = 0x11;
    i2c_write(0x69, 0x7E, &data, 1, 1); // Activación
}



int calibrate_bmi(){
    const int samples = 100;

    uint8_t data_in[6];

    int16_t raw_x, raw_y, raw_z;
    int32_t acc_x, acc_y, acc_z;
    int16_t offset_x = 0, offset_y = 0, offset_z = 0;

    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
;


    for (int i = 0; i < samples; i++) {
        i2c_read(0x69, 0x12, data_in, 6, true, 1);

        raw_x = (int16_t)((data_in[1] << 8) | data_in[0]);
        raw_y = (int16_t)((data_in[3] << 8) | data_in[2]);
        raw_z = (int16_t)((data_in[5] << 8) | data_in[4]);

        sum_x += raw_x;
        sum_y += raw_y;
        sum_z += raw_z;

        my_wait(20);
    }

    offset_x = sum_x / samples;
    offset_y = sum_y / samples;
    offset_z = (sum_z / samples) - 16384;  // subtract +1g for Z if lying flat
}


void read_bmi(uint8_t data_in[6]){


    int16_t raw_x, raw_y, raw_z;
    int32_t acc_x, acc_y, acc_z;
    int16_t offset_x = 0, offset_y = 0, offset_z = 0;

    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
;


    i2c_read(0x69, 0x12, data_in, 6, true, 1);

    raw_x = (int16_t)((data_in[1] << 8) | data_in[0]);
    raw_y = (int16_t)((data_in[3] << 8) | data_in[2]);
    raw_z = (int16_t)((data_in[5] << 8) | data_in[4]);

        // Apply offset
        raw_x -= offset_x;
        raw_y -= offset_y;
        raw_z -= offset_z;

        // Scale to mm/s², fixed point
        acc_x = (raw_x * 9807) / 16384;
        acc_y = (raw_y * 9807) / 16384;
        acc_z = (raw_z * 9807) / 16384;

        printf("a = ( %ld, %ld, %ld ) mm/s^2\n", acc_x, acc_y, acc_z);

}






