#ifndef KALMAN_XYTHETA_VO_H
#define KALMAN_XYTHETA_VO_H

#include <stdint.h>

// === Q16.16 config ===
#define Q16_SHIFT 16
#define Q16_ONE (1 << Q16_SHIFT)

static inline int32_t q16_mul(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a * b) >> Q16_SHIFT);
}

static inline int32_t q16_div(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a << Q16_SHIFT) / b);
}

// === Filter state struct ===
typedef struct {
    int32_t x;       // position X (global)
    int32_t y;       // position Y (global)
    int32_t theta;   // heading
    int32_t v;       // forward linear speed (body frame)
    int32_t omega;   // yaw rate

    int32_t P[5][5]; // Covariance matrix
    int32_t Q[5];    // Process noise (diagonal)
    int32_t R[3];    // Measurement noise (diagonal)
} KalmanXYThetaVO;

// === API ===

void kalman_init(KalmanXYThetaVO *kf);

// Predict with IMU acc_x (body X), gyro_z (yaw rate), dt (Q16.16)
void kalman_predict(KalmanXYThetaVO *kf, int32_t acc_x, int32_t gyro_z, int32_t dt);

// Update with encoder speed (v_enc), encoder yaw (omega_enc), gyro yaw (gyro_z)
void kalman_update(KalmanXYThetaVO *kf, int32_t v_enc, int32_t omega_enc, int32_t gyro_z);

#endif
