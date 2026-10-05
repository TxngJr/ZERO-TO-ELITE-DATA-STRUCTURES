#include <stddef.h>
#include <stdio.h>

static size_t constant_count(const int *data, size_t n) {
    if (n == 0) {
        return 0;
    }
    volatile int sink = data[0];
    (void)sink;
    return 1;
}

static size_t linear_count(const int *data, size_t n) {
    size_t count = 0;
    volatile long long sink = 0;
    for (size_t i = 0; i < n; ++i) {
        sink += data[i];
        ++count;
    }
    (void)sink;
    return count;
}

static size_t triangular_count(size_t n) {
    size_t count = 0;
    volatile size_t sink = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            sink += (i ^ j) & 1U;
            ++count;
        }
    }
    (void)sink;
    return count;
}

static size_t halving_count(size_t n) {
    size_t count = 0;
    while (n > 1) {
        n /= 2;
        ++count;
    }
    return count;
}

int main(void) {
    int data[64];
    for (size_t i = 0; i < 64; ++i) {
        data[i] = (int)i;
    }

    const size_t ns[] = {8, 16, 32, 64};
    for (size_t k = 0; k < sizeof ns / sizeof ns[0]; ++k) {
        const size_t n = ns[k];
        printf("n=%zu constant=%zu linear=%zu triangular=%zu halving=%zu\n",
               n,
               constant_count(data, n),
               linear_count(data, n),
               triangular_count(n),
               halving_count(n));
    }
    return 0;
}
