#include "arena_allocator.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum { ALLOCATIONS = 50000 };

int main(void) {
    Arena *arena = arena_create(1024U);
    assert(arena && arena_validate(arena));

    ArenaMark old_mark = arena_mark(arena);
    for (size_t i = 0; i < ALLOCATIONS; ++i) {
        size_t alignment = (size_t)1U << (i % 13U);
        size_t size = (i % 97U) + 1U;
        void *ptr = arena_alloc(arena, size, alignment);
        assert(ptr);
        assert(((uintptr_t)ptr & (alignment - 1U)) == 0U);
        memset(ptr, (int)(i & 255U), size);
    }

    assert(arena_block_count(arena) > 1U);
    assert(arena_validate(arena));
    size_t reserved_before = arena_reserved_bytes(arena);

    ArenaMark mark = arena_mark(arena);
    void *zeroed = arena_calloc(arena, 256U, 4U, 64U);
    assert(zeroed);
    for (size_t i = 0; i < 1024U; ++i)
        assert(((unsigned char *)zeroed)[i] == 0U);

    assert(arena_reset_to_mark(arena, mark));
    assert(arena_validate(arena));
    assert(!arena_reset_to_mark(arena, mark));
    assert(!arena_reset_to_mark(arena, old_mark));

    arena_reset(arena);
    assert(arena_total_used(arena) == 0U);
    assert(arena_block_count(arena) == 1U);
    assert(arena_reserved_bytes(arena) < reserved_before);
    assert(arena_validate(arena));

    for (size_t alignment = 1U; alignment <= 4096U; alignment *= 2U) {
        void *ptr = arena_alloc(arena, 17U, alignment);
        assert(ptr);
        assert(((uintptr_t)ptr & (alignment - 1U)) == 0U);
    }

    assert(!arena_alloc(arena, 8U, 3U));
    printf("Arena tests passed; blocks=%zu reserved=%zu used=%zu\n",
           arena_block_count(arena),
           arena_reserved_bytes(arena),
           arena_total_used(arena));
    arena_free(arena);
    return 0;
}
