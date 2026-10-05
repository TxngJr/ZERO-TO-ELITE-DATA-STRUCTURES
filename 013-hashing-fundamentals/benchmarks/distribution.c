#include "hash_functions.h"

#include <stdio.h>
#include <stdlib.h>

static void run(size_t bucket_count, size_t key_count) {
    size_t *counts = calloc(bucket_count, sizeof *counts);
    if (counts == NULL) return;

    for (size_t i = 0; i < key_count; ++i) {
        const uint64_t key = (uint64_t)i * 16U;
        size_t bucket = 0;

        if (!hash_bucket(hash_u64_mix(key), bucket_count, &bucket)) {
            free(counts);
            return;
        }

        ++counts[bucket];
    }

    size_t empty = 0;
    size_t min = key_count;
    size_t max = 0;

    for (size_t i = 0; i < bucket_count; ++i) {
        if (counts[i] == 0) ++empty;
        if (counts[i] < min) min = counts[i];
        if (counts[i] > max) max = counts[i];
    }

    printf("buckets=%zu keys=%zu alpha=%.3f empty=%zu min=%zu max=%zu\n",
           bucket_count,
           key_count,
           (double)key_count / (double)bucket_count,
           empty,
           min,
           max);

    free(counts);
}

int main(void) {
    run(64, 10000);
    run(128, 10000);
    run(257, 10000);
    puts("Distribution quality cannot be proven from this small experiment alone.");
    return 0;
}
