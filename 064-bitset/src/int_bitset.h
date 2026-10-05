#ifndef INT_BITSET_H
#define INT_BITSET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBitset IntBitset;

IntBitset *int_bitset_create(size_t bit_count);
void int_bitset_free(IntBitset *set);

size_t int_bitset_size(const IntBitset *set);

bool int_bitset_test(const IntBitset *set,size_t index,bool *out_value);
bool int_bitset_set(IntBitset *set,size_t index);
bool int_bitset_clear(IntBitset *set,size_t index);
bool int_bitset_flip(IntBitset *set,size_t index);

void int_bitset_clear_all(IntBitset *set);
void int_bitset_set_all(IntBitset *set);
void int_bitset_flip_all(IntBitset *set);

size_t int_bitset_count(const IntBitset *set);
bool int_bitset_any(const IntBitset *set);
bool int_bitset_none(const IntBitset *set);
bool int_bitset_all(const IntBitset *set);

bool int_bitset_and(IntBitset *out,const IntBitset *a,const IntBitset *b);
bool int_bitset_or(IntBitset *out,const IntBitset *a,const IntBitset *b);
bool int_bitset_xor(IntBitset *out,const IntBitset *a,const IntBitset *b);
bool int_bitset_not(IntBitset *out,const IntBitset *input);

bool int_bitset_find_next_set(
    const IntBitset *set,
    size_t start,
    size_t *out_index
);

bool int_bitset_validate(const IntBitset *set);

#endif
