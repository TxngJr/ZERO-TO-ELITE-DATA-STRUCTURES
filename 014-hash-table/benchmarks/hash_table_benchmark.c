#include "int_int_hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int key;
    int value;
} Pair;

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static int linear_find(const Pair *pairs, size_t n, int key) {
    for (size_t i = 0; i < n; ++i) {
        if (pairs[i].key == key) return pairs[i].value;
    }
    return -1;
}

int main(void) {
    const size_t ns[] = {1000, 5000, 10000, 20000};

    puts("n,hash_lookup_seconds,linear_lookup_seconds");

    for (size_t t = 0; t < sizeof ns / sizeof ns[0]; ++t) {
        const size_t n = ns[t];
        IntIntHashTable *table = int_int_hash_table_create();
        Pair *pairs = malloc(n * sizeof *pairs);

        if (table == NULL || pairs == NULL) return 1;

        for (size_t i = 0; i < n; ++i) {
            const int key = (int)(i * 2 + 1);
            pairs[i] = (Pair){key, (int)i};
            if (!int_int_hash_table_put(table, key, (int)i)) return 1;
        }

        volatile long long sink = 0;
        struct timespec a, b;

        timespec_get(&a, TIME_UTC);
        for (size_t i = 0; i < n; ++i) {
            int out = 0;
            int_int_hash_table_get(table, pairs[i].key, &out);
            sink += out;
        }
        timespec_get(&b, TIME_UTC);
        const double hash_time = elapsed(a, b);

        timespec_get(&a, TIME_UTC);
        for (size_t i = 0; i < n; ++i) {
            sink += linear_find(pairs, n, pairs[i].key);
        }
        timespec_get(&b, TIME_UTC);
        const double linear_time = elapsed(a, b);

        printf("%zu,%.9f,%.9f\n", n, hash_time, linear_time);

        free(pairs);
        int_int_hash_table_free(table);
        (void)sink;
    }

    puts("Expected hash-table behavior depends on hash distribution and load factor.");
    return 0;
}
