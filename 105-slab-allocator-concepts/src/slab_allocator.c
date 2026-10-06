#include "slab_allocator.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum { CLASS_COUNT = 5 };
static const size_t CLASS_SIZES[CLASS_COUNT] = {
    16U, 32U, 64U, 128U, 256U
};

typedef struct Slab {
    struct Slab *next;
    size_t class_size;
    size_t slots;
    size_t free_top;
    size_t live;
    unsigned char *memory;
    size_t *free_stack;
    unsigned char *in_use;
    unsigned short *requested;
} Slab;

typedef struct {
    size_t class_size;
    Slab *head;
} SlabClass;

struct SlabAllocator {
    size_t slots_per_slab;
    SlabClass classes[CLASS_COUNT];
    size_t live_objects;
    size_t requested_live_bytes;
    size_t allocated_class_bytes;
    size_t reserved_payload_bytes;
    size_t slab_count;
};

static size_t class_index_for(size_t size) {
    for (size_t i = 0; i < CLASS_COUNT; ++i)
        if (size <= CLASS_SIZES[i]) return i;
    return CLASS_COUNT;
}

static Slab *slab_create_one(size_t class_size, size_t slots) {
    if (slots == 0U || class_size > SIZE_MAX / slots ||
        slots > SIZE_MAX / sizeof(size_t) ||
        slots > SIZE_MAX / sizeof(unsigned short))
        return NULL;

    Slab *slab = calloc(1, sizeof(*slab));
    if (!slab) return NULL;

    slab->memory = malloc(class_size * slots);
    slab->free_stack = malloc(slots * sizeof(*slab->free_stack));
    slab->in_use = calloc(slots, 1U);
    slab->requested = calloc(slots, sizeof(*slab->requested));

    if (!slab->memory || !slab->free_stack || !slab->in_use ||
        !slab->requested) {
        free(slab->memory);
        free(slab->free_stack);
        free(slab->in_use);
        free(slab->requested);
        free(slab);
        return NULL;
    }

    slab->class_size = class_size;
    slab->slots = slots;
    slab->free_top = slots;
    for (size_t i = 0; i < slots; ++i)
        slab->free_stack[i] = slots - 1U - i;
    return slab;
}

static void slab_destroy_one(Slab *slab) {
    if (!slab) return;
    free(slab->memory);
    free(slab->free_stack);
    free(slab->in_use);
    free(slab->requested);
    free(slab);
}

SlabAllocator *slab_create(size_t slots_per_slab) {
    if (slots_per_slab == 0U) return NULL;

    SlabAllocator *allocator = calloc(1, sizeof(*allocator));
    if (!allocator) return NULL;

    allocator->slots_per_slab = slots_per_slab;
    for (size_t i = 0; i < CLASS_COUNT; ++i)
        allocator->classes[i].class_size = CLASS_SIZES[i];
    return allocator;
}

void slab_free_allocator(SlabAllocator *allocator) {
    if (!allocator) return;

    for (size_t i = 0; i < CLASS_COUNT; ++i) {
        Slab *slab = allocator->classes[i].head;
        while (slab) {
            Slab *next = slab->next;
            slab_destroy_one(slab);
            slab = next;
        }
    }
    free(allocator);
}

void *slab_alloc(SlabAllocator *allocator, size_t requested_size,
                 size_t *out_class_size) {
    if (!allocator || requested_size == 0U || requested_size > 256U)
        return NULL;

    size_t class_index = class_index_for(requested_size);
    if (class_index == CLASS_COUNT) return NULL;

    Slab *slab = allocator->classes[class_index].head;
    while (slab && slab->free_top == 0U)
        slab = slab->next;

    if (!slab) {
        slab = slab_create_one(
            allocator->classes[class_index].class_size,
            allocator->slots_per_slab);
        if (!slab) return NULL;

        size_t payload = slab->class_size * slab->slots;
        if (allocator->reserved_payload_bytes > SIZE_MAX - payload) {
            slab_destroy_one(slab);
            return NULL;
        }

        slab->next = allocator->classes[class_index].head;
        allocator->classes[class_index].head = slab;
        ++allocator->slab_count;
        allocator->reserved_payload_bytes += payload;
    }

    if (allocator->live_objects == SIZE_MAX ||
        allocator->requested_live_bytes > SIZE_MAX - requested_size ||
        allocator->allocated_class_bytes > SIZE_MAX - slab->class_size)
        return NULL;

    size_t idx = slab->free_stack[--slab->free_top];
    if (idx >= slab->slots || slab->in_use[idx]) {
        ++slab->free_top;
        return NULL;
    }

    slab->in_use[idx] = 1U;
    slab->requested[idx] = (unsigned short)requested_size;
    ++slab->live;
    ++allocator->live_objects;
    allocator->requested_live_bytes += requested_size;
    allocator->allocated_class_bytes += slab->class_size;

    unsigned char *ptr = slab->memory + idx * slab->class_size;
    memset(ptr, 0, slab->class_size);
    if (out_class_size) *out_class_size = slab->class_size;
    return ptr;
}

static bool locate(SlabAllocator *allocator, void *ptr,
                   Slab **out_slab, size_t *out_index) {
    if (!allocator || !ptr) return false;
    uintptr_t p = (uintptr_t)ptr;

    for (size_t class_index = 0; class_index < CLASS_COUNT;
         ++class_index) {
        for (Slab *slab = allocator->classes[class_index].head;
             slab; slab = slab->next) {
            uintptr_t base = (uintptr_t)slab->memory;
            size_t bytes = slab->class_size * slab->slots;
            if (p < base || p >= base + bytes) continue;

            uintptr_t delta = p - base;
            if (delta % slab->class_size != 0U) return false;
            size_t idx = (size_t)(delta / slab->class_size);
            if (idx >= slab->slots) return false;

            *out_slab = slab;
            *out_index = idx;
            return true;
        }
    }
    return false;
}

bool slab_release(SlabAllocator *allocator, void *ptr) {
    Slab *slab = NULL;
    size_t idx = 0U;
    if (!locate(allocator, ptr, &slab, &idx)) return false;
    if (!slab->in_use[idx] || slab->free_top >= slab->slots)
        return false;

    size_t requested = slab->requested[idx];
    slab->in_use[idx] = 0U;
    slab->requested[idx] = 0U;
    slab->free_stack[slab->free_top++] = idx;
    --slab->live;
    --allocator->live_objects;
    allocator->requested_live_bytes -= requested;
    allocator->allocated_class_bytes -= slab->class_size;
    return true;
}

size_t slab_trim_empty(SlabAllocator *allocator) {
    if (!allocator) return 0U;
    size_t trimmed = 0U;

    for (size_t class_index = 0; class_index < CLASS_COUNT;
         ++class_index) {
        Slab **link = &allocator->classes[class_index].head;
        while (*link) {
            Slab *slab = *link;
            if (slab->live == 0U) {
                *link = slab->next;
                allocator->reserved_payload_bytes -=
                    slab->class_size * slab->slots;
                --allocator->slab_count;
                ++trimmed;
                slab_destroy_one(slab);
            } else {
                link = &slab->next;
            }
        }
    }
    return trimmed;
}

size_t slab_live_objects(const SlabAllocator *a) {
    return a ? a->live_objects : 0U;
}
size_t slab_requested_live_bytes(const SlabAllocator *a) {
    return a ? a->requested_live_bytes : 0U;
}
size_t slab_allocated_class_bytes(const SlabAllocator *a) {
    return a ? a->allocated_class_bytes : 0U;
}
size_t slab_reserved_payload_bytes(const SlabAllocator *a) {
    return a ? a->reserved_payload_bytes : 0U;
}
size_t slab_slab_count(const SlabAllocator *a) {
    return a ? a->slab_count : 0U;
}

bool slab_validate(const SlabAllocator *allocator) {
    if (!allocator || allocator->slots_per_slab == 0U) return false;

    size_t live = 0U, requested = 0U, allocated = 0U;
    size_t reserved = 0U, slabs = 0U;

    for (size_t class_index = 0; class_index < CLASS_COUNT;
         ++class_index) {
        if (allocator->classes[class_index].class_size !=
            CLASS_SIZES[class_index])
            return false;

        for (Slab *slab = allocator->classes[class_index].head;
             slab; slab = slab->next) {
            if (slab->class_size != CLASS_SIZES[class_index] ||
                slab->slots != allocator->slots_per_slab ||
                !slab->memory || !slab->free_stack || !slab->in_use ||
                !slab->requested || slab->free_top > slab->slots ||
                slab->live > slab->slots ||
                slab->free_top + slab->live != slab->slots)
                return false;

            unsigned char *seen = calloc(slab->slots, 1U);
            if (!seen) return false;

            size_t slab_live = 0U;
            for (size_t i = 0; i < slab->slots; ++i) {
                if (!slab->in_use[i]) continue;
                ++slab_live;

                if (slab->requested[i] == 0U ||
                    slab->requested[i] > slab->class_size) {
                    free(seen);
                    return false;
                }

                if (requested > SIZE_MAX - slab->requested[i] ||
                    allocated > SIZE_MAX - slab->class_size) {
                    free(seen);
                    return false;
                }
                requested += slab->requested[i];
                allocated += slab->class_size;
            }

            for (size_t i = 0; i < slab->free_top; ++i) {
                size_t idx = slab->free_stack[i];
                if (idx >= slab->slots || seen[idx] ||
                    slab->in_use[idx]) {
                    free(seen);
                    return false;
                }
                seen[idx] = 1U;
            }
            free(seen);

            if (slab_live != slab->live) return false;
            if (live > SIZE_MAX - slab->live ||
                reserved > SIZE_MAX - slab->class_size * slab->slots ||
                slabs == SIZE_MAX)
                return false;

            live += slab->live;
            reserved += slab->class_size * slab->slots;
            ++slabs;
        }
    }

    return live == allocator->live_objects &&
           requested == allocator->requested_live_bytes &&
           allocated == allocator->allocated_class_bytes &&
           reserved == allocator->reserved_payload_bytes &&
           slabs == allocator->slab_count;
}
