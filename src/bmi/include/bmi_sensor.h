

#ifndef _BMI_SENSOR_H
#define _BMI_SENSOR_H

#include <stdint.h>

int init_bmi();
int calibrate_bmi();

void read_bmi(uint8_t data_in[6]);

#endif
