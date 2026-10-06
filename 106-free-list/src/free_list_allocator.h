#ifndef FREE_LIST_ALLOCATOR_H
#define FREE_LIST_ALLOCATOR_H
#include <stdbool.h>
#include <stddef.h>
typedef struct FreeListAllocator FreeListAllocator;
FreeListAllocator *fla_create(size_t capacity);
void fla_free(FreeListAllocator *allocator);
void *fla_alloc(FreeListAllocator *allocator,size_t size,size_t alignment);
bool fla_release(FreeListAllocator *allocator,void *ptr);
size_t fla_capacity(const FreeListAllocator *allocator);
size_t fla_allocated_bytes(const FreeListAllocator *allocator);
size_t fla_free_bytes(const FreeListAllocator *allocator);
size_t fla_allocation_count(const FreeListAllocator *allocator);
size_t fla_free_block_count(const FreeListAllocator *allocator);
size_t fla_largest_free_block(const FreeListAllocator *allocator);
bool fla_validate(const FreeListAllocator *allocator);
#endif
