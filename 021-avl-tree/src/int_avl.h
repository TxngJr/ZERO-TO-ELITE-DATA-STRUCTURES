#ifndef INT_AVL_H
#define INT_AVL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntAVL IntAVL;

IntAVL *int_avl_create(void);
void int_avl_free(IntAVL *tree);

size_t int_avl_size(const IntAVL *tree);
bool int_avl_contains(const IntAVL *tree, int key);
bool int_avl_insert(IntAVL *tree, int key);
bool int_avl_remove(IntAVL *tree, int key);

bool int_avl_height(const IntAVL *tree, size_t *out_edge_height);
bool int_avl_inorder(const IntAVL *tree, int *output, size_t capacity, size_t *out_written);
bool int_avl_validate(const IntAVL *tree);

#endif
