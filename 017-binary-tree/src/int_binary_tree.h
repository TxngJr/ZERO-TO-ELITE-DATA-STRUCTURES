#ifndef INT_BINARY_TREE_H
#define INT_BINARY_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBinaryNode IntBinaryNode;
typedef struct IntBinaryTree IntBinaryTree;

IntBinaryTree *int_binary_tree_create(void);
void int_binary_tree_free(IntBinaryTree *tree);

size_t int_binary_tree_size(const IntBinaryTree *tree);
IntBinaryNode *int_binary_tree_root(const IntBinaryTree *tree);

bool int_binary_tree_set_root(IntBinaryTree *tree, int value, IntBinaryNode **out_node);
bool int_binary_tree_add_left(IntBinaryTree *tree, IntBinaryNode *parent, int value, IntBinaryNode **out_node);
bool int_binary_tree_add_right(IntBinaryTree *tree, IntBinaryNode *parent, int value, IntBinaryNode **out_node);
bool int_binary_tree_set_value(IntBinaryTree *tree, IntBinaryNode *node, int value);
bool int_binary_tree_remove_subtree(IntBinaryTree *tree, IntBinaryNode *node);

int int_binary_node_value(const IntBinaryNode *node);
IntBinaryNode *int_binary_node_parent(const IntBinaryNode *node);
IntBinaryNode *int_binary_node_left(const IntBinaryNode *node);
IntBinaryNode *int_binary_node_right(const IntBinaryNode *node);

bool int_binary_tree_height(const IntBinaryTree *tree, size_t *out_height);
bool int_binary_tree_contains_node(const IntBinaryTree *tree, const IntBinaryNode *node);
bool int_binary_tree_validate(const IntBinaryTree *tree);

#endif
