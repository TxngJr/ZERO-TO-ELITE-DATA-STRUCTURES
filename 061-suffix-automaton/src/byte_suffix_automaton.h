#ifndef BYTE_SUFFIX_AUTOMATON_H
#define BYTE_SUFFIX_AUTOMATON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteSuffixAutomaton ByteSuffixAutomaton;

ByteSuffixAutomaton *byte_suffix_automaton_create(
    const uint8_t *text,
    size_t length
);

void byte_suffix_automaton_free(ByteSuffixAutomaton *automaton);

size_t byte_suffix_automaton_text_length(
    const ByteSuffixAutomaton *automaton
);

size_t byte_suffix_automaton_state_count(
    const ByteSuffixAutomaton *automaton
);

bool byte_suffix_automaton_contains(
    const ByteSuffixAutomaton *automaton,
    const uint8_t *pattern,
    size_t pattern_length
);

bool byte_suffix_automaton_count(
    const ByteSuffixAutomaton *automaton,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
);

bool byte_suffix_automaton_distinct_substrings(
    const ByteSuffixAutomaton *automaton,
    uint64_t *out_count
);

bool byte_suffix_automaton_longest_repeated(
    const ByteSuffixAutomaton *automaton,
    size_t *out_length
);

bool byte_suffix_automaton_validate(
    const ByteSuffixAutomaton *automaton
);

#endif
