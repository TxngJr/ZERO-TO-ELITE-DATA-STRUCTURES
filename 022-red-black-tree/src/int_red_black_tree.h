#ifndef INT_RED_BLACK_TREE_H
#define INT_RED_BLACK_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntRedBlackTree IntRedBlackTree;

IntRedBlackTree *int_rbt_create(void);
void int_rbt_free(IntRedBlackTree *tree);

size_t int_rbt_size(const IntRedBlackTree *tree);
bool int_rbt_contains(const IntRedBlackTree *tree, int key);
bool int_rbt_insert(IntRedBlackTree *tree, int key);
bool int_rbt_remove(IntRedBlackTree *tree, int key);

bool int_rbt_height(const IntRedBlackTree *tree, size_t *out_edge_height);
bool int_rbt_inorder(const IntRedBlackTree *tree, int *output, size_t capacity, size_t *out_written);
bool int_rbt_validate(const IntRedBlackTree *tree);

#endif
