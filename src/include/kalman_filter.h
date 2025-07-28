#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include <stdint.h>

// Estado del filtro en Q16.16
typedef struct {
    int32_t pos_x;
    int32_t pos_y;
    int32_t vel_x;
    int32_t vel_y;
    int32_t theta;
} KalmanState;

void kalman_init(KalmanState* state);
void kalman_predict(KalmanState* state, int32_t acc_x, int32_t acc_y, int32_t gyro_z_deg, int32_t dt_q16);
void kalman_print(const KalmanState* state);

#endif
