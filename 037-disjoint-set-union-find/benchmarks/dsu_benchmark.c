#include "int_dsu.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const size_t n = 200000;
    const size_t operations = 1000000;

    IntDSU *dsu = int_dsu_create(n);
    if (dsu == NULL) return 1;

    uint32_t rng = 123456789u;
    bool merged = false;

    for (size_t i = 1; i < n; ++i) {
        if (!int_dsu_union(dsu, i - 1, i, &merged)) return 1;
    }

    size_t depth_before = 0;
    if (!int_dsu_max_depth(dsu, &depth_before)) return 1;

    struct timespec a;
    struct timespec b;

    timespec_get(&a, TIME_UTC);

    volatile size_t checksum = 0;

    for (size_t i = 0; i < operations; ++i) {
        rng = rng * 1664525u + 1013904223u;
        size_t root = 0;

        if (!int_dsu_find(dsu, (size_t)rng % n, &root)) return 1;
        checksum ^= root;
    }

    timespec_get(&b, TIME_UTC);

    size_t depth_after = 0;
    if (!int_dsu_max_depth(dsu, &depth_after)) return 1;

    printf(
        "n=%zu operations=%zu seconds=%.9f max_depth_before=%zu max_depth_after=%zu checksum=%zu\n",
        n,
        operations,
        elapsed(a, b),
        depth_before,
        depth_after,
        (size_t)checksum
    );

    int_dsu_free(dsu);
    return 0;
}
