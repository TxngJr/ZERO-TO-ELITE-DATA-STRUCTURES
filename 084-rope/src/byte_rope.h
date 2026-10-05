#ifndef BYTE_ROPE_H
#define BYTE_ROPE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct ByteRope ByteRope;
ByteRope *byte_rope_create(uint64_t seed);
void byte_rope_free(ByteRope *rope);
size_t byte_rope_size(const ByteRope *rope);
bool byte_rope_insert(ByteRope *rope,size_t position,const uint8_t *bytes,size_t length);
bool byte_rope_erase(ByteRope *rope,size_t position,size_t length);
bool byte_rope_get(const ByteRope *rope,size_t index,uint8_t *out_byte);
bool byte_rope_copy(const ByteRope *rope,uint8_t *out,size_t out_capacity);
bool byte_rope_validate(const ByteRope *rope);
#endif
