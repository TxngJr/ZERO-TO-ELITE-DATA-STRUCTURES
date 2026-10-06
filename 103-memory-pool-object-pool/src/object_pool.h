#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
#include <stdbool.h>
#include <stddef.h>
typedef struct ObjectPool ObjectPool;
ObjectPool *op_create(size_t object_size, size_t capacity);
void op_free(ObjectPool *pool);
void *op_alloc(ObjectPool *pool);
bool op_release(ObjectPool *pool, void *ptr);
bool op_owns(const ObjectPool *pool, const void *ptr);
size_t op_capacity(const ObjectPool *pool);
size_t op_object_size(const ObjectPool *pool);
size_t op_stride(const ObjectPool *pool);
size_t op_in_use_count(const ObjectPool *pool);
bool op_validate(const ObjectPool *pool);
#endif
