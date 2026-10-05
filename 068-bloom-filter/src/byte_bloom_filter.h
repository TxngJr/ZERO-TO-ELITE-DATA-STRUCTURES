#ifndef BYTE_BLOOM_FILTER_H
#define BYTE_BLOOM_FILTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteBloomFilter ByteBloomFilter;

ByteBloomFilter *byte_bloom_filter_create(
    size_t bit_count,
    size_t hash_count
);

void byte_bloom_filter_free(ByteBloomFilter *filter);

size_t byte_bloom_filter_bit_count(const ByteBloomFilter *filter);
size_t byte_bloom_filter_hash_count(const ByteBloomFilter *filter);
size_t byte_bloom_filter_insertions(const ByteBloomFilter *filter);
size_t byte_bloom_filter_set_bits(const ByteBloomFilter *filter);

bool byte_bloom_filter_add(
    ByteBloomFilter *filter,
    const uint8_t *key,
    size_t key_length
);

bool byte_bloom_filter_maybe_contains(
    const ByteBloomFilter *filter,
    const uint8_t *key,
    size_t key_length,
    bool *out_maybe
);

bool byte_bloom_filter_validate(const ByteBloomFilter *filter);

#endif
