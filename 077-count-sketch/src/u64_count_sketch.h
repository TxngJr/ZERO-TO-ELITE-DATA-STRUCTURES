#ifndef U64_COUNT_SKETCH_H
#define U64_COUNT_SKETCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64CountSketch U64CountSketch;

U64CountSketch *u64_count_sketch_create(size_t width,size_t depth);
void u64_count_sketch_free(U64CountSketch *sketch);

size_t u64_count_sketch_width(const U64CountSketch *sketch);
size_t u64_count_sketch_depth(const U64CountSketch *sketch);

bool u64_count_sketch_update(U64CountSketch *sketch,uint64_t key,int64_t delta);
bool u64_count_sketch_estimate(const U64CountSketch *sketch,uint64_t key,int64_t *out_estimate);
bool u64_count_sketch_merge(U64CountSketch *out,const U64CountSketch *other);
bool u64_count_sketch_validate(const U64CountSketch *sketch);

#endif
