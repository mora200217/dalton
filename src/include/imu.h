#ifndef IMU_H
#define IMU_H

#include <stdint.h>

void imu_init(void);


void imu_calibrate(int16_t* offset_x, int16_t* offset_y, int16_t* offset_z);
void imu_read_acc(int16_t* x, int16_t* y, int16_t* z);
int16_t imu_read_gyro_z(void);

static int16_t offset_x;
static int16_t offset_y;
static int16_t offset_z; 
 

void imu_set_offsets(int16_t ox, int16_t oy, int16_t oz); 
void imu_read_acc(int16_t* x, int16_t* y, int16_t* z); 

#endif // IMU_H
