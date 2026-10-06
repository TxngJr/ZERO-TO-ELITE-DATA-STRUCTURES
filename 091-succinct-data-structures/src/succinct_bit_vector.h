#ifndef SUCCINCT_BIT_VECTOR_H
#define SUCCINCT_BIT_VECTOR_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct SuccinctBitVector SuccinctBitVector;

/* bits must contain exactly nbits bytes, each equal to 0 or 1. */
SuccinctBitVector *sbv_create(const uint8_t *bits, size_t nbits);
void sbv_free(SuccinctBitVector *bv);

size_t sbv_size(const SuccinctBitVector *bv);
size_t sbv_ones(const SuccinctBitVector *bv);
size_t sbv_storage_bytes(const SuccinctBitVector *bv);

bool sbv_get(const SuccinctBitVector *bv, size_t index, bool *out_bit);
/* rank counts in the half-open prefix [0, end). end may equal size. */
bool sbv_rank1(const SuccinctBitVector *bv, size_t end, size_t *out_rank);
bool sbv_rank0(const SuccinctBitVector *bv, size_t end, size_t *out_rank);
/* kth is zero-based among matching bits. */
bool sbv_select1(const SuccinctBitVector *bv, size_t kth, size_t *out_index);
bool sbv_select0(const SuccinctBitVector *bv, size_t kth, size_t *out_index);

bool sbv_validate(const SuccinctBitVector *bv);
#endif
