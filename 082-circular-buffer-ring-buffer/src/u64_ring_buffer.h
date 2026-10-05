#ifndef U64_RING_BUFFER_H
#define U64_RING_BUFFER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct U64RingBuffer U64RingBuffer;
U64RingBuffer *u64_ring_buffer_create(size_t capacity);
void u64_ring_buffer_free(U64RingBuffer *buffer);
size_t u64_ring_buffer_capacity(const U64RingBuffer *buffer);
size_t u64_ring_buffer_size(const U64RingBuffer *buffer);
bool u64_ring_buffer_is_empty(const U64RingBuffer *buffer);
bool u64_ring_buffer_is_full(const U64RingBuffer *buffer);
bool u64_ring_buffer_push(U64RingBuffer *buffer,uint64_t value);
bool u64_ring_buffer_pop(U64RingBuffer *buffer,uint64_t *out_value);
bool u64_ring_buffer_front(const U64RingBuffer *buffer,uint64_t *out_value);
bool u64_ring_buffer_back(const U64RingBuffer *buffer,uint64_t *out_value);
bool u64_ring_buffer_get(const U64RingBuffer *buffer,size_t logical_index,uint64_t *out_value);
void u64_ring_buffer_clear(U64RingBuffer *buffer);
bool u64_ring_buffer_validate(const U64RingBuffer *buffer);
#endif
