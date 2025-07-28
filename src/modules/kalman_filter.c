#include "kalman_filter.h"
#include <stdio.h>

#define DEG2RAD_Q16 1147 // Q16.16 approx of π/180

void kalman_init(KalmanState* state) {
    state->pos_x = 0;
    state->pos_y = 0;
    state->vel_x = 0;
    state->vel_y = 0;
    state->theta = 0;
}

void kalman_predict(KalmanState* state, int32_t acc_x, int32_t acc_y, int32_t gyro_z_deg, int32_t dt_q16) {
    // Convert gyro to Q16.16 rad/s
    int32_t gyro_z_q16 = gyro_z_deg * DEG2RAD_Q16;

    // Integrate orientation
    state->theta += ((int64_t)gyro_z_q16 * dt_q16) >> 16;

    // Convert acc from mm/s² to m/s² in Q16.16
    int32_t acc_x_q16 = (acc_x * 65536) / 1000;
    int32_t acc_y_q16 = (acc_y * 65536) / 1000;

    // Integrate acceleration to velocity
    state->vel_x += ((int64_t)acc_x_q16 * dt_q16) >> 16;
    state->vel_y += ((int64_t)acc_y_q16 * dt_q16) >> 16;

    // Integrate velocity to position
    state->pos_x += ((int64_t)state->vel_x * dt_q16) >> 16;
    state->pos_y += ((int64_t)state->vel_y * dt_q16) >> 16;
}

void kalman_print(const KalmanState* state) {
    int draw_x = state->pos_x >> 16;
    int draw_y = state->pos_y >> 16;

    if (draw_x < 0) draw_x = 0;
    if (draw_x > 40) draw_x = 40;
    if (draw_y < 0) draw_y = 0;
    if (draw_y > 10) draw_y = 10;

    printf("\033[2J"); // clear screen
    for (int y = 10; y >= 0; y--) {
        for (int x = 0; x <= 40; x++) {
            if (x == draw_x && y == draw_y) {
                printf("O");
            } else if (y == 0 && x == 0) {
                printf("+");
            } else if (y == 0) {
                printf("-");
            } else if (x == 0) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    int32_t x_int = state->pos_x >> 16;
    uint32_t x_frac = ((state->pos_x & 0xFFFF) * 1000) >> 16;

    int32_t y_int = state->pos_y >> 16;
    uint32_t y_frac = ((state->pos_y & 0xFFFF) * 1000) >> 16;

    int32_t theta_deg = (state->theta * 180) / (DEG2RAD_Q16 * 65536);

    printf("Pos: (%ld.%03u, %ld.%03u) m  Angulo: %ld deg\n",
        x_int, x_frac, y_int, y_frac, theta_deg);
}
