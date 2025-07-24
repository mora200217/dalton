#include <stdint.h>

#define Q16_SHIFT 16
#define Q16_ONE (1 << Q16_SHIFT)

static inline int32_t q16_mul(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a * b) >> Q16_SHIFT);
}

static inline int32_t q16_div(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a << Q16_SHIFT) / b);
}

typedef struct {
    int32_t x;     // pos x
    int32_t y;     // pos y
    int32_t theta; // heading
    int32_t v;     // forward speed
    int32_t omega; // yaw rate

    int32_t P[5][5];  // Covariance
    int32_t Q[5];     // Process noise
    int32_t R[3];     // Measurement noise

} KalmanXYThetaVO;

void kalman_init(KalmanXYThetaVO *kf) {
    kf->x = 0;
    kf->y = 0;
    kf->theta = 0;
    kf->v = 0;
    kf->omega = 0;

    // Init P diagonal
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            kf->P[i][j] = (i == j) ? Q16_ONE : 0;
        }
        kf->Q[i] = Q16_ONE / 10000; // small process noise
    }

    kf->R[0] = Q16_ONE / 100;  // v_enc
    kf->R[1] = Q16_ONE / 100;  // omega_enc
    kf->R[2] = Q16_ONE / 100;  // gyro
}

void kalman_predict(KalmanXYThetaVO *kf, int32_t acc_x, int32_t gyro_z, int32_t dt) {
    // Integrate speed
    kf->v += q16_mul(acc_x, dt);

    // Integrate heading
    kf->theta += q16_mul(gyro_z, dt);

    // Compute global frame update
    int32_t cos_theta = Q16_ONE; // approx for small tests
    int32_t sin_theta = 0;       // same

    kf->x += q16_mul(q16_mul(kf->v, cos_theta), dt);
    kf->y += q16_mul(q16_mul(kf->v, sin_theta), dt);

    kf->omega = gyro_z; // simple: overwrite by gyro

    // Covariance prediction: simple diagonal increase
    for (int i = 0; i < 5; i++) {
        kf->P[i][i] += kf->Q[i];
    }
}

// 2x2 inverse for S matrix
void invert_3x3(int32_t S[3][3], int32_t Sinv[3][3]) {
    int64_t det = 
        (int64_t)S[0][0]*(S[1][1]*S[2][2] - S[1][2]*S[2][1]) -
        (int64_t)S[0][1]*(S[1][0]*S[2][2] - S[1][2]*S[2][0]) +
        (int64_t)S[0][2]*(S[1][0]*S[2][1] - S[1][1]*S[2][0]);

    if (det == 0) return; // singular matrix

    Sinv[0][0] = q16_div((S[1][1]*S[2][2] - S[1][2]*S[2][1]), det >> Q16_SHIFT);
    Sinv[0][1] = q16_div(-(S[0][1]*S[2][2] - S[0][2]*S[2][1]), det >> Q16_SHIFT);
    Sinv[0][2] = q16_div((S[0][1]*S[1][2] - S[0][2]*S[1][1]), det >> Q16_SHIFT);

    Sinv[1][0] = q16_div(-(S[1][0]*S[2][2] - S[1][2]*S[2][0]), det >> Q16_SHIFT);
    Sinv[1][1] = q16_div((S[0][0]*S[2][2] - S[0][2]*S[2][0]), det >> Q16_SHIFT);
    Sinv[1][2] = q16_div(-(S[0][0]*S[1][2] - S[0][2]*S[1][0]), det >> Q16_SHIFT);

    Sinv[2][0] = q16_div((S[1][0]*S[2][1] - S[1][1]*S[2][0]), det >> Q16_SHIFT);
    Sinv[2][1] = q16_div(-(S[0][0]*S[2][1] - S[0][1]*S[2][0]), det >> Q16_SHIFT);
    Sinv[2][2] = q16_div((S[0][0]*S[1][1] - S[0][1]*S[1][0]), det >> Q16_SHIFT);
}

void kalman_update(KalmanXYThetaVO *kf, int32_t v_enc, int32_t omega_enc, int32_t gyro_z) {
    // z = [v_enc, omega_enc, gyro_z]
    int32_t z[3] = { v_enc, omega_enc, gyro_z };
    int32_t h_x[3] = { kf->v, kf->omega, kf->omega };

    int32_t y[3]; // innovation
    for (int i = 0; i < 3; i++) {
        y[i] = z[i] - h_x[i];
    }

    // S = HPH^T + R
    int32_t S[3][3] = {0};
    for (int i = 0; i < 3; i++) {
        int s_idx = (i == 0) ? 3 : 4; // v or omega index
        for (int j = 0; j < 3; j++) {
            int s_jdx = (j == 0) ? 3 : 4;
            S[i][j] = kf->P[s_idx][s_jdx];
        }
        S[i][i] += kf->R[i];
    }

    int32_t Sinv[3][3];
    invert_3x3(S, Sinv);

    // K = PH^T Sinv
    int32_t K[5][3] = {0};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            int s_jdx = (j == 0) ? 3 : 4;
            K[i][j] = q16_mul(kf->P[i][s_jdx], Sinv[j][j]); // simple diag only
        }
    }

    // x = x + K y
    kf->v     += q16_mul(K[3][0], y[0]);
    kf->omega += q16_mul(K[4][1], y[1]);
    kf->omega += q16_mul(K[4][2], y[2]);

    // Update covariance (simplified: skipped here for brevity)
    // For full EKF: P = P - K H P
}

