#include "object_pool.h"
#include <stdio.h>
#include <time.h>

enum { CAPACITY = 4096, TARGET = 1000000 };

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1e9;
}

int main(void) {
    ObjectPool *pool = op_create(32U, CAPACITY);
    if (!pool) return 1;

    void *items[CAPACITY];
    struct timespec begin, end;
    timespec_get(&begin, TIME_UTC);

    int rounds = TARGET / CAPACITY;
    for (int round = 0; round < rounds; ++round) {
        for (int i = 0; i < CAPACITY; ++i) {
            items[i] = op_alloc(pool);
            if (!items[i]) return 2;
        }
        for (int i = 0; i < CAPACITY; ++i)
            if (!op_release(pool, items[i])) return 3;
    }

    timespec_get(&end, TIME_UTC);
    printf("operations=%d seconds=%.6f\n",
           rounds * CAPACITY * 2,
           elapsed(begin, end));

    op_free(pool);
    return 0;
}
