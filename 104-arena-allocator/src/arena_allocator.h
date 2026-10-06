#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Arena Arena;

typedef struct {
    void *block_token;
    size_t used;
    size_t total_used;
    size_t block_count;
    uint64_t generation;
} ArenaMark;

Arena *arena_create(size_t default_block_size);
void arena_free(Arena *arena);
void *arena_alloc(Arena *arena, size_t size, size_t alignment);
void *arena_calloc(Arena *arena, size_t count, size_t size,
                   size_t alignment);
ArenaMark arena_mark(const Arena *arena);
bool arena_reset_to_mark(Arena *arena, ArenaMark mark);
void arena_reset(Arena *arena);
size_t arena_total_used(const Arena *arena);
size_t arena_block_count(const Arena *arena);
size_t arena_reserved_bytes(const Arena *arena);
bool arena_validate(const Arena *arena);
#endif
