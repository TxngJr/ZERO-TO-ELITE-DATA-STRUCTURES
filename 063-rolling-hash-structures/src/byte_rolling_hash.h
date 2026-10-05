#ifndef BYTE_ROLLING_HASH_H
#define BYTE_ROLLING_HASH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t first;
    uint64_t second;
} ByteHashPair;

typedef struct ByteRollingHash ByteRollingHash;

ByteRollingHash *byte_rolling_hash_create(
    const uint8_t *text,
    size_t length
);

void byte_rolling_hash_free(ByteRollingHash *hash);

size_t byte_rolling_hash_size(const ByteRollingHash *hash);

bool byte_rolling_hash_range(
    const ByteRollingHash *hash,
    size_t left,
    size_t right,
    ByteHashPair *out_hash
);

bool byte_rolling_hash_equal(
    const ByteRollingHash *hash,
    size_t left_a,
    size_t right_a,
    size_t left_b,
    size_t right_b,
    bool *out_equal
);

bool byte_rolling_hash_lcp(
    const ByteRollingHash *hash,
    size_t start_a,
    size_t start_b,
    size_t max_length,
    size_t *out_lcp
);

bool byte_rolling_hash_contains(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length
);

bool byte_rolling_hash_count(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
);

bool byte_rolling_hash_report(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
);

bool byte_rolling_hash_validate(const ByteRollingHash *hash);

#endif
