#ifndef SLAB_ALLOCATOR_H
#define SLAB_ALLOCATOR_H
#include <stdbool.h>
#include <stddef.h>

typedef struct SlabAllocator SlabAllocator;

SlabAllocator *slab_create(size_t slots_per_slab);
void slab_free_allocator(SlabAllocator *allocator);
void *slab_alloc(SlabAllocator *allocator, size_t requested_size,
                 size_t *out_class_size);
bool slab_release(SlabAllocator *allocator, void *ptr);
size_t slab_trim_empty(SlabAllocator *allocator);
size_t slab_live_objects(const SlabAllocator *allocator);
size_t slab_requested_live_bytes(const SlabAllocator *allocator);
size_t slab_allocated_class_bytes(const SlabAllocator *allocator);
size_t slab_reserved_payload_bytes(const SlabAllocator *allocator);
size_t slab_slab_count(const SlabAllocator *allocator);
bool slab_validate(const SlabAllocator *allocator);

#endif
