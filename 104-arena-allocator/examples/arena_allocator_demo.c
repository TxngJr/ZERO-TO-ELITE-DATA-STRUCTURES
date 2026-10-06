#include "arena_allocator.h"
#include <stdio.h>

int main(void) {
    Arena *arena = arena_create(256U);
    if (!arena) return 1;

    int *values = arena_alloc(arena, 32U * sizeof(*values), 16U);
    if (!values) return 2;

    ArenaMark mark = arena_mark(arena);
    char *temporary = arena_alloc(arena, 600U, 64U);
    if (!temporary) return 3;
    temporary[0] = 'A';

    printf("blocks=%zu used=%zu temporary=%c\n",
           arena_block_count(arena),
           arena_total_used(arena),
           temporary[0]);

    if (!arena_reset_to_mark(arena, mark)) return 4;
    arena_free(arena);
    return 0;
}
