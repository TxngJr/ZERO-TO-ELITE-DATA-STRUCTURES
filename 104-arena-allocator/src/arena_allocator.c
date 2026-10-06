#include "arena_allocator.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { ARENA_MAX_ALIGNMENT = 4096 };

typedef struct ArenaBlock {
    struct ArenaBlock *next;
    size_t capacity;
    size_t used;
    unsigned char *data;
    unsigned char raw[];
} ArenaBlock;

struct Arena {
    size_t default_block_size;
    ArenaBlock *head;
    size_t total_used;
    size_t block_count;
    size_t reserved_bytes;
    uint64_t generation;
};

static bool is_pow2(size_t x) {
    return x != 0U && (x & (x - 1U)) == 0U;
}

static uintptr_t align_up_uintptr(uintptr_t value, size_t alignment) {
    return (value + (uintptr_t)(alignment - 1U)) &
           ~(uintptr_t)(alignment - 1U);
}

static ArenaBlock *block_create(size_t capacity) {
    if (capacity == 0U) return NULL;
    if (capacity > SIZE_MAX - (size_t)(ARENA_MAX_ALIGNMENT - 1U) -
                       sizeof(ArenaBlock))
        return NULL;

    size_t bytes = sizeof(ArenaBlock) + capacity +
                   (ARENA_MAX_ALIGNMENT - 1U);
    ArenaBlock *block = malloc(bytes);
    if (!block) return NULL;

    block->next = NULL;
    block->capacity = capacity;
    block->used = 0U;
    uintptr_t raw = (uintptr_t)block->raw;
    block->data = (unsigned char *)
        align_up_uintptr(raw, ARENA_MAX_ALIGNMENT);
    return block;
}

Arena *arena_create(size_t default_block_size) {
    if (default_block_size == 0U) return NULL;

    Arena *arena = calloc(1, sizeof(*arena));
    if (!arena) return NULL;

    ArenaBlock *block = block_create(default_block_size);
    if (!block) {
        free(arena);
        return NULL;
    }

    arena->default_block_size = default_block_size;
    arena->head = block;
    arena->block_count = 1U;
    arena->reserved_bytes = default_block_size;
    arena->generation = 1U;
    return arena;
}

void arena_free(Arena *arena) {
    if (!arena) return;
    while (arena->head) {
        ArenaBlock *next = arena->head->next;
        free(arena->head);
        arena->head = next;
    }
    free(arena);
}

static bool block_try_alloc(ArenaBlock *block, size_t size,
                            size_t alignment, size_t *out_offset) {
    if (block->used > block->capacity) return false;

    uintptr_t base = (uintptr_t)block->data;
    uintptr_t current = base + block->used;
    uintptr_t aligned = align_up_uintptr(current, alignment);
    if (aligned < current) return false;

    size_t offset = (size_t)(aligned - base);
    if (offset > block->capacity) return false;
    if (size > block->capacity - offset) return false;

    *out_offset = offset;
    return true;
}

void *arena_alloc(Arena *arena, size_t size, size_t alignment) {
    if (!arena || size == 0U || !is_pow2(alignment) ||
        alignment > ARENA_MAX_ALIGNMENT)
        return NULL;

    size_t offset = 0U;
    if (!block_try_alloc(arena->head, size, alignment, &offset)) {
        size_t min_capacity = size;
        if (min_capacity > SIZE_MAX - (alignment - 1U))
            return NULL;
        min_capacity += alignment - 1U;

        size_t capacity = arena->default_block_size > min_capacity
                            ? arena->default_block_size
                            : min_capacity;
        ArenaBlock *block = block_create(capacity);
        if (!block) return NULL;

        if (arena->reserved_bytes > SIZE_MAX - capacity) {
            free(block);
            return NULL;
        }

        block->next = arena->head;
        arena->head = block;
        ++arena->block_count;
        arena->reserved_bytes += capacity;

        if (!block_try_alloc(arena->head, size, alignment, &offset)) {
            arena->head = block->next;
            --arena->block_count;
            arena->reserved_bytes -= capacity;
            free(block);
            return NULL;
        }
    }

    size_t previous = arena->head->used;
    arena->head->used = offset + size;
    size_t consumed = arena->head->used - previous;
    if (arena->total_used > SIZE_MAX - consumed) {
        arena->head->used = previous;
        return NULL;
    }

    arena->total_used += consumed;
    return arena->head->data + offset;
}

void *arena_calloc(Arena *arena, size_t count, size_t size,
                   size_t alignment) {
    if (count && size > SIZE_MAX / count) return NULL;
    size_t total = count * size;
    if (total == 0U) return NULL;

    void *ptr = arena_alloc(arena, total, alignment);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

ArenaMark arena_mark(const Arena *arena) {
    ArenaMark mark = {0};
    if (!arena) return mark;

    mark.block_token = arena->head;
    mark.used = arena->head ? arena->head->used : 0U;
    mark.total_used = arena->total_used;
    mark.block_count = arena->block_count;
    mark.generation = arena->generation;
    return mark;
}

bool arena_reset_to_mark(Arena *arena, ArenaMark mark) {
    if (!arena || mark.generation != arena->generation ||
        !mark.block_token)
        return false;

    ArenaBlock *target = (ArenaBlock *)mark.block_token;
    ArenaBlock *scan = arena->head;
    bool found = false;
    while (scan) {
        if (scan == target) {
            found = true;
            break;
        }
        scan = scan->next;
    }
    if (!found || mark.used > target->capacity) return false;

    while (arena->head != target) {
        ArenaBlock *dead = arena->head;
        arena->head = dead->next;
        arena->reserved_bytes -= dead->capacity;
        --arena->block_count;
        free(dead);
    }

    arena->head->used = mark.used;
    arena->total_used = mark.total_used;
    arena->block_count = mark.block_count;
    ++arena->generation;
    if (arena->generation == 0U) ++arena->generation;
    return true;
}

void arena_reset(Arena *arena) {
    if (!arena) return;

    while (arena->head && arena->head->next) {
        ArenaBlock *dead = arena->head;
        arena->head = dead->next;
        free(dead);
    }

    if (arena->head) {
        arena->head->used = 0U;
        arena->reserved_bytes = arena->head->capacity;
        arena->block_count = 1U;
    } else {
        arena->reserved_bytes = 0U;
        arena->block_count = 0U;
    }

    arena->total_used = 0U;
    ++arena->generation;
    if (arena->generation == 0U) ++arena->generation;
}

size_t arena_total_used(const Arena *arena) {
    return arena ? arena->total_used : 0U;
}
size_t arena_block_count(const Arena *arena) {
    return arena ? arena->block_count : 0U;
}
size_t arena_reserved_bytes(const Arena *arena) {
    return arena ? arena->reserved_bytes : 0U;
}

bool arena_validate(const Arena *arena) {
    if (!arena || !arena->head || arena->default_block_size == 0U ||
        arena->generation == 0U)
        return false;

    size_t blocks = 0U, reserved = 0U, used = 0U;
    for (ArenaBlock *block = arena->head; block; block = block->next) {
        if (!block->data || block->capacity == 0U ||
            block->used > block->capacity)
            return false;
        if (((uintptr_t)block->data % ARENA_MAX_ALIGNMENT) != 0U)
            return false;
        if (blocks == SIZE_MAX || reserved > SIZE_MAX - block->capacity ||
            used > SIZE_MAX - block->used)
            return false;
        ++blocks;
        reserved += block->capacity;
        used += block->used;
    }

    return blocks == arena->block_count &&
           reserved == arena->reserved_bytes &&
           used == arena->total_used;
}
