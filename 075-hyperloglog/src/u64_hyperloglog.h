#ifndef U64_HYPERLOGLOG_H
#define U64_HYPERLOGLOG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64HyperLogLog U64HyperLogLog;

U64HyperLogLog *u64_hll_create(unsigned precision);
void u64_hll_free(U64HyperLogLog *hll);

unsigned u64_hll_precision(const U64HyperLogLog *hll);
size_t u64_hll_register_count(const U64HyperLogLog *hll);

bool u64_hll_add(U64HyperLogLog *hll,uint64_t value);
bool u64_hll_merge(U64HyperLogLog *out,const U64HyperLogLog *other);
bool u64_hll_estimate(const U64HyperLogLog *hll,double *out_estimate);
bool u64_hll_validate(const U64HyperLogLog *hll);

#endif
