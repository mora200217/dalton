// Multiplica matriz A (3x3) por vector x (3x1), resultado en y
void mat_vec_mul(const mat3 A, const vec3 x, vec3 y) {
    for (int i = 0; i < N; i++) {
        y[i] = 0;
        for (int j = 0; j < N; j++) {
            y[i] += (A[i][j] * x[j]) >> Q;
        }
    }
}

// Multiplica matrices A (3x3) * B (3x3), resultado en C
void mat_mul(const mat3 A, const mat3 B, mat3 C) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += (A[i][k] * B[k][j]) >> Q;
        }
}

// Transpuesta de matriz (3x1 → 1x3)
void transpose_H(int32_t H[M][N], int32_t Ht[N][M]) {
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            Ht[j][i] = H[i][j];
}

// Multiplica vector transpuesto (1x3) por vector (3x1)
int32_t dot_product_q15(const int32_t *a, const int32_t *b, int len) {
    int32_t result = 0;
    for (int i = 0; i < len; i++) {
        result += (a[i] * b[i]) >> Q;
    }
    return result;
}
