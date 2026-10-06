#include "slab_allocator.h"
#include <stdio.h>

int main(void) {
    SlabAllocator *allocator = slab_create(32U);
    if (!allocator) return 1;

    size_t class_size = 0U;
    void *ptr = slab_alloc(allocator, 37U, &class_size);
    if (!ptr) return 2;

    printf("request=37 class=%zu live=%zu fragmentation=%zu\n",
           class_size,
           slab_live_objects(allocator),
           slab_allocated_class_bytes(allocator) -
           slab_requested_live_bytes(allocator));

    if (!slab_release(allocator, ptr)) return 3;
    slab_trim_empty(allocator);
    slab_free_allocator(allocator);
    return 0;
}
