#ifndef INT_SPLAY_TREE_H
#define INT_SPLAY_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntSplayTree IntSplayTree;

IntSplayTree *int_splay_create(void);
void int_splay_free(IntSplayTree *tree);

size_t int_splay_size(const IntSplayTree *tree);
bool int_splay_insert(IntSplayTree *tree, int key);
bool int_splay_contains(IntSplayTree *tree, int key);
bool int_splay_remove(IntSplayTree *tree, int key);

bool int_splay_root_key(const IntSplayTree *tree, int *out_key);
bool int_splay_height(const IntSplayTree *tree, size_t *out_edge_height);
bool int_splay_inorder(const IntSplayTree *tree, int *output, size_t capacity, size_t *out_written);
bool int_splay_validate(const IntSplayTree *tree);

#endif
