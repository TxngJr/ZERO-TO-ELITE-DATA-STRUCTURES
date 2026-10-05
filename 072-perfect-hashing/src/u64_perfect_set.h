#ifndef U64_PERFECT_SET_H
#define U64_PERFECT_SET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64PerfectSet U64PerfectSet;

U64PerfectSet *u64_perfect_set_create(const uint64_t *keys,size_t count);
void u64_perfect_set_free(U64PerfectSet *set);
size_t u64_perfect_set_size(const U64PerfectSet *set);
size_t u64_perfect_set_bucket_count(const U64PerfectSet *set);
size_t u64_perfect_set_secondary_slots(const U64PerfectSet *set);
bool u64_perfect_set_contains(const U64PerfectSet *set,uint64_t key);
bool u64_perfect_set_validate(const U64PerfectSet *set);

#endif
