#ifndef BYTE_COUNTING_BLOOM_H
#define BYTE_COUNTING_BLOOM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteCountingBloom ByteCountingBloom;

ByteCountingBloom *byte_counting_bloom_create(
    size_t counter_count,
    size_t hash_count
);

void byte_counting_bloom_free(ByteCountingBloom *filter);

size_t byte_counting_bloom_counter_count(const ByteCountingBloom *filter);
size_t byte_counting_bloom_hash_count(const ByteCountingBloom *filter);
size_t byte_counting_bloom_logical_count(const ByteCountingBloom *filter);
size_t byte_counting_bloom_nonzero_counters(const ByteCountingBloom *filter);

bool byte_counting_bloom_add(
    ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length
);

bool byte_counting_bloom_remove(
    ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length
);

bool byte_counting_bloom_maybe_contains(
    const ByteCountingBloom *filter,
    const uint8_t *key,
    size_t key_length,
    bool *out_maybe
);

bool byte_counting_bloom_validate(const ByteCountingBloom *filter);

#endif
