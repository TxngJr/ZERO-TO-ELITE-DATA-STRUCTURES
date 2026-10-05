#ifndef INT_BTREE_H
#define INT_BTREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBTree IntBTree;

IntBTree *int_btree_create(size_t minimum_degree);
void int_btree_free(IntBTree *tree);

size_t int_btree_size(const IntBTree *tree);
size_t int_btree_minimum_degree(const IntBTree *tree);

bool int_btree_contains(const IntBTree *tree, int key);
bool int_btree_insert(IntBTree *tree, int key);
bool int_btree_remove(IntBTree *tree, int key);

bool int_btree_height(const IntBTree *tree, size_t *out_edge_height);
bool int_btree_inorder(const IntBTree *tree, int *output, size_t capacity, size_t *out_written);
bool int_btree_validate(const IntBTree *tree);

#endif
