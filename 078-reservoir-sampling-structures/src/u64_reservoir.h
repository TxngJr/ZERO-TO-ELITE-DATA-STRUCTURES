#ifndef U64_RESERVOIR_H
#define U64_RESERVOIR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64Reservoir U64Reservoir;

U64Reservoir *u64_reservoir_create(size_t capacity,uint64_t seed);
void u64_reservoir_free(U64Reservoir *reservoir);

size_t u64_reservoir_capacity(const U64Reservoir *reservoir);
size_t u64_reservoir_sample_count(const U64Reservoir *reservoir);
uint64_t u64_reservoir_seen(const U64Reservoir *reservoir);

bool u64_reservoir_add(U64Reservoir *reservoir,uint64_t value);
bool u64_reservoir_get(const U64Reservoir *reservoir,size_t index,uint64_t *out_value);
bool u64_reservoir_validate(const U64Reservoir *reservoir);

#endif
