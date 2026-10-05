#ifndef INT_TREAP_H
#define INT_TREAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntTreap IntTreap;

IntTreap *int_treap_create(void);
void int_treap_free(IntTreap *tree);

size_t int_treap_size(const IntTreap *tree);
bool int_treap_contains(const IntTreap *tree, int key);
bool int_treap_insert(IntTreap *tree, int key);
bool int_treap_insert_with_priority(IntTreap *tree, int key, uint32_t priority);
bool int_treap_remove(IntTreap *tree, int key);

bool int_treap_root(const IntTreap *tree, int *out_key, uint32_t *out_priority);
bool int_treap_height(const IntTreap *tree, size_t *out_edge_height);
bool int_treap_inorder(const IntTreap *tree, int *output, size_t capacity, size_t *out_written);
bool int_treap_validate(const IntTreap *tree);

#endif
