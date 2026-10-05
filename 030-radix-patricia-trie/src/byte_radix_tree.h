#ifndef BYTE_RADIX_TREE_H
#define BYTE_RADIX_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ByteRadixTree ByteRadixTree;

typedef bool (*byte_radix_visit_fn)(
    const unsigned char *bytes,
    size_t length,
    void *context
);

ByteRadixTree *byte_radix_tree_create(void);
void byte_radix_tree_free(ByteRadixTree *tree);

size_t byte_radix_tree_size(const ByteRadixTree *tree);
size_t byte_radix_tree_node_count(const ByteRadixTree *tree);

bool byte_radix_tree_insert(ByteRadixTree *tree, const unsigned char *bytes, size_t length);
bool byte_radix_tree_contains(const ByteRadixTree *tree, const unsigned char *bytes, size_t length);
bool byte_radix_tree_has_prefix(const ByteRadixTree *tree, const unsigned char *bytes, size_t length);
size_t byte_radix_tree_count_prefix(const ByteRadixTree *tree, const unsigned char *bytes, size_t length);
bool byte_radix_tree_remove(ByteRadixTree *tree, const unsigned char *bytes, size_t length);

bool byte_radix_tree_visit_prefix(
    const ByteRadixTree *tree,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_radix_visit_fn visitor,
    void *context
);

bool byte_radix_tree_validate(const ByteRadixTree *tree);

#endif
