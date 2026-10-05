#ifndef U64_KMV_SKETCH_H
#define U64_KMV_SKETCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64KmvSketch U64KmvSketch;

U64KmvSketch *u64_kmv_sketch_create(size_t k);
void u64_kmv_sketch_free(U64KmvSketch *sketch);

size_t u64_kmv_sketch_k(const U64KmvSketch *sketch);
size_t u64_kmv_sketch_retained(const U64KmvSketch *sketch);

bool u64_kmv_sketch_add(U64KmvSketch *sketch,uint64_t value);
bool u64_kmv_sketch_merge(U64KmvSketch *out,const U64KmvSketch *other);
bool u64_kmv_sketch_estimate(const U64KmvSketch *sketch,double *out_estimate);
bool u64_kmv_sketch_validate(const U64KmvSketch *sketch);

#endif
