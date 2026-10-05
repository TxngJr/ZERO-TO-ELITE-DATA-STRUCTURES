#ifndef INT_BPLUS_TREE_H
#define INT_BPLUS_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBPlusTree IntBPlusTree;

IntBPlusTree *int_bplus_tree_create(size_t minimum_degree);
void int_bplus_tree_free(IntBPlusTree *tree);

size_t int_bplus_tree_size(const IntBPlusTree *tree);
bool int_bplus_tree_contains(const IntBPlusTree *tree, int key);
bool int_bplus_tree_insert(IntBPlusTree *tree, int key);
bool int_bplus_tree_remove(IntBPlusTree *tree, int key);

bool int_bplus_tree_range(
    const IntBPlusTree *tree,
    int low,
    int high,
    int *output,
    size_t capacity,
    size_t *out_written
);

bool int_bplus_tree_height(const IntBPlusTree *tree, size_t *out_edge_height);
bool int_bplus_tree_validate(const IntBPlusTree *tree);

#endif
