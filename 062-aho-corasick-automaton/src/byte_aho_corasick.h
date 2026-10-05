#ifndef BYTE_AHO_CORASICK_H
#define BYTE_AHO_CORASICK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    const uint8_t *bytes;
    size_t length;
    uint64_t id;
} BytePattern;

typedef struct {
    uint64_t pattern_id;
    size_t end_position;
} ByteACMatch;

typedef struct ByteAhoCorasick ByteAhoCorasick;

ByteAhoCorasick *byte_aho_corasick_create(
    const BytePattern *patterns,
    size_t pattern_count
);

void byte_aho_corasick_free(ByteAhoCorasick *automaton);

size_t byte_aho_corasick_state_count(
    const ByteAhoCorasick *automaton
);

size_t byte_aho_corasick_pattern_count(
    const ByteAhoCorasick *automaton
);

bool byte_aho_corasick_count_matches(
    const ByteAhoCorasick *automaton,
    const uint8_t *text,
    size_t text_length,
    size_t *out_count
);

bool byte_aho_corasick_report(
    const ByteAhoCorasick *automaton,
    const uint8_t *text,
    size_t text_length,
    ByteACMatch *output,
    size_t capacity,
    size_t *out_written
);

bool byte_aho_corasick_validate(
    const ByteAhoCorasick *automaton
);

#endif
