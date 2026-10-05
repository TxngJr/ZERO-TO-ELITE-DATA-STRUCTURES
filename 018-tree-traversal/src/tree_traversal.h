#ifndef TREE_TRAVERSAL_H
#define TREE_TRAVERSAL_H

#include "int_binary_tree.h"

#include <stdbool.h>
#include <stddef.h>

bool tree_preorder_recursive(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);
bool tree_inorder_recursive(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);
bool tree_postorder_recursive(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);

bool tree_preorder_iterative(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);
bool tree_inorder_iterative(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);
bool tree_postorder_iterative(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);

bool tree_level_order(const IntBinaryTree *tree, int *output, size_t capacity, size_t *out_written);

#endif
