#include "int_btree.h"
#include "int_skip_list.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(
    struct timespec a,
    struct timespec b
) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) /
               1000000000.0;
}

int main(void) {
    const int n = 50000;
    const int probes = 500000;

    IntSkipList *skip =
        int_skip_list_create(32);
    IntBTree *btree =
        int_btree_create(32);

    if (skip == NULL || btree == NULL) return 1;

    uint32_t rng = 123456789u;

    for (int i = 0; i < n; ++i) {
        rng = rng * 1664525u + 1013904223u;
        const int key = (int)(rng & 0x7fffffffU);

        if (!int_skip_list_contains(skip, key)) {
            if (!int_skip_list_insert(skip, key)) {
                return 1;
            }

            if (!int_btree_insert(btree, key)) {
                return 1;
            }
        }
    }

    volatile int hits = 0;
    struct timespec a;
    struct timespec b;

    rng = 987654321u;

    timespec_get(&a, TIME_UTC);

    for (int i = 0; i < probes; ++i) {
        rng = rng * 1664525u + 1013904223u;
        hits += int_skip_list_contains(
            skip,
            (int)(rng & 0x7fffffffU)
        ) ? 1 : 0;
    }

    timespec_get(&b, TIME_UTC);
    const double skip_time = elapsed(a, b);

    rng = 987654321u;

    timespec_get(&a, TIME_UTC);

    for (int i = 0; i < probes; ++i) {
        rng = rng * 1664525u + 1013904223u;
        hits += int_btree_contains(
            btree,
            (int)(rng & 0x7fffffffU)
        ) ? 1 : 0;
    }

    timespec_get(&b, TIME_UTC);
    const double btree_time = elapsed(a, b);

    printf(
        "size_skip=%zu active_levels=%zu skip_lookup_s=%.9f btree_lookup_s=%.9f\n",
        int_skip_list_size(skip),
        int_skip_list_current_level(skip),
        skip_time,
        btree_time
    );

    puts("This is a teaching comparison; layout, fanout, allocator and CPU cache dominate practical constants.");

    (void)hits;
    int_skip_list_free(skip);
    int_btree_free(btree);
    return 0;
}
