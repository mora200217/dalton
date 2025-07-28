#include "imu.h"
#include <stdio.h>
#include <i2c.h>

#include "utils.h"

void imu_init(void) {
    unsigned char data;

    data = 0x11;
    i2c_write(0x68, 0x7E, &data, 1, 1);  // Activar accelerómetro
    my_wait(10);

    data = 0x15;
    i2c_write(0x68, 0x7E, &data, 1, 1);  // Activar giroscopio
    my_wait(10);

    printf("IMU initialized\n");
}

void imu_calibrate(int16_t* offset_x, int16_t* offset_y, int16_t* offset_z) {
    uint8_t data_in[6];
    int64_t sum_x = 0, sum_y = 0, sum_z = 0;
    const int samples = 100;

    printf("Calibrating IMU...\n");
    for (int i = 0; i < samples; i++) {
        i2c_read(0x68, 0x12, data_in, 6, true, 1);

        sum_x += (int16_t)((data_in[1] << 8) | data_in[0]);
        sum_y += (int16_t)((data_in[3] << 8) | data_in[2]);
        sum_z += (int16_t)((data_in[5] << 8) | data_in[4]);

        my_wait(20);
    }

    *offset_x = sum_x / samples;
    *offset_y = sum_y / samples;
    *offset_z = (sum_z / samples) - 16384;  // Ajuste por gravedad

    printf("IMU offsets: x=%d, y=%d, z=%d\n", *offset_x, *offset_y, *offset_z);
}


int16_t imu_read_gyro_z(void) {
    uint8_t gyro_z[2];
    i2c_read(0x68, 0x10, gyro_z, 2, true, 1);
    return (int16_t)((gyro_z[1] << 8) | gyro_z[0]);
}


static int16_t offset_x;
static int16_t offset_y;
static int16_t offset_z; 

void imu_set_offsets(int16_t ox, int16_t oy, int16_t oz) {
    offset_x = ox;
    offset_y = oy;
    offset_z = oz;
}

void imu_read_acc(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t data_in[6];

    i2c_read(0x68, 0x12, data_in, 6, true, 1);
    *x = (int16_t)((data_in[1] << 8) | data_in[0]) - offset_x;
    *y = (int16_t)((data_in[3] << 8) | data_in[2]) - offset_y;
    *z = (int16_t)((data_in[5] << 8) | data_in[4]) - offset_z;
}