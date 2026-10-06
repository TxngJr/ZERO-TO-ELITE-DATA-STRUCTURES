#ifndef ATOMIC_STRUCTURES_H
#define ATOMIC_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct AtomicBitset AtomicBitset;
typedef struct AtomicTaggedValue AtomicTaggedValue;
typedef struct { uint32_t value; uint32_t version; } ATVSnapshot;
AtomicBitset *abs_create(size_t nbits);
void abs_free(AtomicBitset *set);
size_t abs_size(const AtomicBitset *set);
bool abs_platform_lock_free(const AtomicBitset *set);
bool abs_test(const AtomicBitset *set,size_t index,bool *out_value);
bool abs_test_and_set(AtomicBitset *set,size_t index,bool *out_previous);
bool abs_test_and_clear(AtomicBitset *set,size_t index,bool *out_previous);
bool abs_test_and_toggle(AtomicBitset *set,size_t index,bool *out_previous);
bool abs_compare_exchange_word(AtomicBitset *set,size_t word_index,uint64_t *expected,uint64_t desired,bool *out_swapped);
size_t abs_count_quiescent(const AtomicBitset *set);
bool abs_validate_quiescent(const AtomicBitset *set);
AtomicTaggedValue *atv_create(uint32_t initial_value);
void atv_free(AtomicTaggedValue *value);
bool atv_platform_lock_free(const AtomicTaggedValue *value);
bool atv_load(const AtomicTaggedValue *value,ATVSnapshot *out);
bool atv_compare_exchange(AtomicTaggedValue *value,ATVSnapshot *expected,uint32_t desired_value,bool *out_swapped);
bool atv_fetch_increment(AtomicTaggedValue *value,uint32_t *out_previous,size_t *out_retries);
#endif
