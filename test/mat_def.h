// Dimensiones
#define N 3   // Tamaño del estado
#define M 1   // Tamaño de la medición

#define Q 15
#define ONE_Q15 (1 << Q)

typedef int32_t mat3[N][N];
typedef int32_t vec3[N];
typedef int32_t vec1[M];

// Matriz de transición (A)
// A = [1 dt dt^2/2
//      0  1   dt
//      0  0   1 ]
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
