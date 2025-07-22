#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>  // rand()

#define Q 15
#define ONE_Q15 (1 << Q)

#define N 3 // número de estados: posición, velocidad, aceleración
#define M 1 // número de mediciones

typedef int32_t mat3[N][N];
typedef int32_t vec3[N];
typedef int32_t vec1[M];

// --- Agrega aquí tus funciones auxiliares de matrices y kalman_predict_update (omitidas aquí por espacio) ---

void init_A(mat3 A, int32_t dt_q15) {
    A[0][0] = ONE_Q15;
    A[0][1] = dt_q15;
    A[0][2] = (dt_q15 * dt_q15) >> 1;

    A[1][0] = 0;
    A[1][1] = ONE_Q15;
    A[1][2] = dt_q15;

    A[2][0] = 0;
    A[2][1] = 0;
    A[2][2] = ONE_Q15;
}

int32_t dot_product_q15(const int32_t *a, const int32_t *b, int len) {
    int32_t result = 0;
    for (int i = 0; i < len; i++) {
        result += (a[i] * b[i]) >> Q;
    }
    return result;
}

void mat_vec_mul(const mat3 A, const vec3 x, vec3 y) {
    for (int i = 0; i < N; i++) {
        y[i] = 0;
        for (int j = 0; j < N; j++) {
            y[i] += (A[i][j] * x[j]) >> Q;
        }
    }
}

void mat_mul(const mat3 A, const mat3 B, mat3 C) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += (A[i][k] * B[k][j]) >> Q;
        }
}

void kalman_predict_update(
    vec3 x,
    mat3 P,
    const mat3 A,
    const int32_t H[1][N],
    int32_t R,
    int32_t z_measured
) {
    vec3 x_pred;
    mat3 P_pred;

    // x = A * x
    mat_vec_mul(A, x, x_pred);

    // P = A * P * A^T
    mat3 temp;
    mat_mul(A, P, temp);
    mat_mul(temp, A, P_pred);

    // Innovation: y = z - H * x
    int32_t Hx = dot_product_q15(H[0], x_pred, N);
    int32_t y = (z_measured << Q) - Hx;

    // S = H * P * H^T + R
    int32_t HP[N];
    for (int i = 0; i < N; i++)
        HP[i] = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            HP[i] += (H[0][j] * P_pred[j][i]) >> Q;
    int32_t S = dot_product_q15(H[0], HP, N) + R;

    // K = P * H^T / S
    int32_t K[N];
    for (int i = 0; i < N; i++) {
        K[i] = HP[i] / S;
    }

    // x = x + K * y
    for (int i = 0; i < N; i++)
        x[i] = x_pred[i] + ((K[i] * y) >> Q);

    // P = (I - K * H) * P
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int32_t kh = (K[i] * H[0][j]) >> Q;
            int32_t delta = (i == j) ? ONE_Q15 : 0;
            P[i][j] = ((delta - kh) * P_pred[i][j]) >> Q;
        }
}

int main(int argc, char* argv[]) {
    printf("Prueba filtro de Kalman\n");
    printf("--\n");

    vec3 x = {0}; // estado inicial: posición, velocidad, aceleración
    mat3 P = {
        {ONE_Q15, 0, 0},
        {0, ONE_Q15, 0},
        {0, 0, ONE_Q15}
    };

    mat3 A;
    int32_t dt = ONE_Q15; // dt = 1.0s en Q15
    init_A(A, dt);

    // Medimos solo la posición
    int32_t H[1][N] = {
        {ONE_Q15, 0, 0}
    };

    // Ruido de medición
    int32_t R = 262144; // 8.0 en Q15

    int32_t true_position = 0;
    int32_t velocity = 6 << Q; // 2 m/s
    int32_t acceleration = 4  << Q; // 1 m/s^2

    while (1) {
        sleep(1);
        // printf("siguiente iteración...\n");

        // Simular movimiento con aceleración constante
        true_position += (velocity >> Q); // posición += velocidad
        velocity += (acceleration >> Q);  // velocidad += aceleración

        // Simular medición con ruido
        int32_t noise = (rand() % (1 << Q)) - (1 << (Q - 1)); // ruido +/- 0.5
        int32_t measurement = true_position + (noise >> Q);

        // Ejecutar Kalman
        kalman_predict_update(x, P, A, H, R, measurement);

        // Imprimir resultados
        printf("Medición: %d\tEstimado: %d\tVelocidad: %d\tAceleración: %d\n",
               measurement,
               x[0] >> Q,
               x[1] >> Q,
               x[2] >> Q);
    }

    return 0;
}
