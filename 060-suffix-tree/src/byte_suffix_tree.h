#ifndef BYTE_SUFFIX_TREE_H
#define BYTE_SUFFIX_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteSuffixTree ByteSuffixTree;

ByteSuffixTree *byte_suffix_tree_create(
    const uint8_t *text,
    size_t length
);

void byte_suffix_tree_free(ByteSuffixTree *tree);

size_t byte_suffix_tree_text_length(const ByteSuffixTree *tree);
size_t byte_suffix_tree_node_count(const ByteSuffixTree *tree);

bool byte_suffix_tree_contains(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length
);

bool byte_suffix_tree_count(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
);

bool byte_suffix_tree_report(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
);

bool byte_suffix_tree_validate(const ByteSuffixTree *tree);

#endif
