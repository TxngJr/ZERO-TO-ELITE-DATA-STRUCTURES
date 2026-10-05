#ifndef INT_BIT_VECTOR_H
#define INT_BIT_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntBitVector IntBitVector;

IntBitVector *int_bit_vector_create(
    const uint8_t *bits,
    size_t bit_count
);

void int_bit_vector_free(IntBitVector *vector);

size_t int_bit_vector_size(const IntBitVector *vector);
size_t int_bit_vector_ones(const IntBitVector *vector);
size_t int_bit_vector_zeros(const IntBitVector *vector);

bool int_bit_vector_access(
    const IntBitVector *vector,
    size_t index,
    bool *out_bit
);

bool int_bit_vector_rank1(
    const IntBitVector *vector,
    size_t end,
    size_t *out_rank
);

bool int_bit_vector_rank0(
    const IntBitVector *vector,
    size_t end,
    size_t *out_rank
);

bool int_bit_vector_select1(
    const IntBitVector *vector,
    size_t k,
    size_t *out_index
);

bool int_bit_vector_select0(
    const IntBitVector *vector,
    size_t k,
    size_t *out_index
);

bool int_bit_vector_validate(const IntBitVector *vector);

#endif
