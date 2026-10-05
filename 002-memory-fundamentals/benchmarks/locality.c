#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { N = 1024 };

static double seconds_since(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void) {
    int *matrix = malloc((size_t)N * N * sizeof *matrix);
    if (matrix == NULL) {
        return 1;
    }

    for (size_t i = 0; i < (size_t)N * N; ++i) {
        matrix[i] = (int)(i & 255U);
    }

    volatile int64_t row_sum = 0;
    volatile int64_t col_sum = 0;
    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            row_sum += matrix[r * N + c];
        }
    }
    timespec_get(&b, TIME_UTC);
    double row_time = seconds_since(a, b);

    timespec_get(&a, TIME_UTC);
    for (size_t c = 0; c < N; ++c) {
        for (size_t r = 0; r < N; ++r) {
            col_sum += matrix[r * N + c];
        }
    }
    timespec_get(&b, TIME_UTC);
    double col_time = seconds_since(a, b);

    printf("row-major sum=%lld time=%.6f s\n", (long long)row_sum, row_time);
    printf("column-major sum=%lld time=%.6f s\n", (long long)col_sum, col_time);
    puts("Interpret trends across repeated runs; do not treat one timing as a universal constant.");

    free(matrix);
    return row_sum == col_sum ? 0 : 1;
}
