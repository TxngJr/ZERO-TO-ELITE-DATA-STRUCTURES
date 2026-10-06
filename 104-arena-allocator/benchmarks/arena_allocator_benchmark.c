#include "arena_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { ALLOCATIONS = 1000000 };

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void) {
    Arena *arena = arena_create(65536U);
    if (!arena) return 1;
    void **ptrs = malloc((size_t)ALLOCATIONS * sizeof(*ptrs));
    if (!ptrs) return 2;

    struct timespec begin, allocated, reset;
    timespec_get(&begin, TIME_UTC);
    for (int i = 0; i < ALLOCATIONS; ++i) {
        ptrs[i] = arena_alloc(arena, 24U, 8U);
        if (!ptrs[i]) return 3;
    }
    timespec_get(&allocated, TIME_UTC);
    arena_reset(arena);
    timespec_get(&reset, TIME_UTC);

    printf("allocations=%d alloc_seconds=%.6f reset_seconds=%.9f\n",
           ALLOCATIONS,
           elapsed(begin, allocated),
           elapsed(allocated, reset));

    free(ptrs);
    arena_free(arena);
    return 0;
}
