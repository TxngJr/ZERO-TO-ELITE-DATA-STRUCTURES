#ifndef U64_COUNT_MIN_SKETCH_H
#define U64_COUNT_MIN_SKETCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct U64CountMinSketch U64CountMinSketch;

U64CountMinSketch *u64_cms_create(size_t width,size_t depth);
void u64_cms_free(U64CountMinSketch *sketch);

size_t u64_cms_width(const U64CountMinSketch *sketch);
size_t u64_cms_depth(const U64CountMinSketch *sketch);
uint64_t u64_cms_total_weight(const U64CountMinSketch *sketch);

bool u64_cms_add(U64CountMinSketch *sketch,uint64_t key,uint64_t delta);
bool u64_cms_estimate(const U64CountMinSketch *sketch,uint64_t key,uint64_t *out_estimate);
bool u64_cms_merge(U64CountMinSketch *out,const U64CountMinSketch *other);
bool u64_cms_validate(const U64CountMinSketch *sketch);

#endif
