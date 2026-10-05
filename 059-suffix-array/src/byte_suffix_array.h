#ifndef BYTE_SUFFIX_ARRAY_H
#define BYTE_SUFFIX_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteSuffixArray ByteSuffixArray;

ByteSuffixArray *byte_suffix_array_create(
    const uint8_t *text,
    size_t length
);

void byte_suffix_array_free(ByteSuffixArray *array);

size_t byte_suffix_array_size(const ByteSuffixArray *array);

bool byte_suffix_array_at(
    const ByteSuffixArray *array,
    size_t rank,
    size_t *out_suffix_start
);

bool byte_suffix_array_rank_of(
    const ByteSuffixArray *array,
    size_t suffix_start,
    size_t *out_rank
);

bool byte_suffix_array_lcp_at(
    const ByteSuffixArray *array,
    size_t rank,
    size_t *out_lcp
);

bool byte_suffix_array_contains(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length
);

bool byte_suffix_array_count(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
);

bool byte_suffix_array_report(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
);

bool byte_suffix_array_validate(const ByteSuffixArray *array);

#endif
