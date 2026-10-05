#include <stddef.h>
#include <stdio.h>

static long long sum_array(const int *data, size_t n) {
    long long sum = 0;
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
    }
    return sum;
}

int main(void) {
    int a[5] = {10, 20, 30, 40, 50};

    printf("sizeof array=%zu sizeof pointer=%zu\n", sizeof a, sizeof &a[0]);
    for (size_t i = 0; i < 5; ++i) {
        printf("a[%zu]=%d address=%p\n", i, a[i], (void *)&a[i]);
    }

    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    puts("matrix addresses in row-major iteration:");
    for (size_t r = 0; r < 2; ++r) {
        for (size_t c = 0; c < 3; ++c) {
            printf("m[%zu][%zu]=%d address=%p\n",
                   r, c, matrix[r][c], (void *)&matrix[r][c]);
        }
    }

    printf("sum(a)=%lld\n", sum_array(a, 5));
    return sum_array(a, 5) == 150 ? 0 : 1;
}
