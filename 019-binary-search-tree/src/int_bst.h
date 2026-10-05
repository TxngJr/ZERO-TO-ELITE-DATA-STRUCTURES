#ifndef INT_BST_H
#define INT_BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBSTNode IntBSTNode;
typedef struct IntBST IntBST;

IntBST *int_bst_create(void);
void int_bst_free(IntBST *tree);

size_t int_bst_size(const IntBST *tree);
IntBSTNode *int_bst_root(const IntBST *tree);

bool int_bst_insert(IntBST *tree, int key, IntBSTNode **out_node);
IntBSTNode *int_bst_find(const IntBST *tree, int key);
bool int_bst_contains(const IntBST *tree, int key);
bool int_bst_remove(IntBST *tree, int key);

IntBSTNode *int_bst_minimum(const IntBST *tree);
IntBSTNode *int_bst_maximum(const IntBST *tree);
IntBSTNode *int_bst_successor(const IntBSTNode *node);
IntBSTNode *int_bst_predecessor(const IntBSTNode *node);

int int_bst_node_key(const IntBSTNode *node);
IntBSTNode *int_bst_node_left(const IntBSTNode *node);
IntBSTNode *int_bst_node_right(const IntBSTNode *node);
IntBSTNode *int_bst_node_parent(const IntBSTNode *node);

bool int_bst_height(const IntBST *tree, size_t *out_height);
bool int_bst_inorder(const IntBST *tree, int *output, size_t capacity, size_t *out_written);
bool int_bst_validate(const IntBST *tree);

#endif
