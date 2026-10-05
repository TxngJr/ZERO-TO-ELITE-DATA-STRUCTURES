#ifndef ROT_BST_H
#define ROT_BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct RotBSTNode RotBSTNode;
typedef struct RotBST RotBST;

RotBST *rot_bst_create(void);
void rot_bst_free(RotBST *tree);
size_t rot_bst_size(const RotBST *tree);
RotBSTNode *rot_bst_root(const RotBST *tree);
RotBSTNode *rot_bst_find(const RotBST *tree, int key);

bool rot_bst_insert(RotBST *tree, int key);
bool rot_bst_rotate_left(RotBST *tree, int pivot_key);
bool rot_bst_rotate_right(RotBST *tree, int pivot_key);

int rot_bst_node_key(const RotBSTNode *node);
bool rot_bst_height(const RotBST *tree, size_t *out_height);
bool rot_bst_inorder(const RotBST *tree, int *output, size_t capacity, size_t *out_written);
bool rot_bst_validate(const RotBST *tree);

#endif
