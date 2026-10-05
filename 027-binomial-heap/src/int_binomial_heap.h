#ifndef INT_BINOMIAL_HEAP_H
#define INT_BINOMIAL_HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBinomialNode IntBinomialNode;
typedef struct IntBinomialHeap IntBinomialHeap;

IntBinomialHeap *int_binomial_heap_create(void);
void int_binomial_heap_free(IntBinomialHeap *heap);

size_t int_binomial_heap_size(const IntBinomialHeap *heap);
IntBinomialNode *int_binomial_heap_insert(IntBinomialHeap *heap, int key);
bool int_binomial_heap_peek_min(const IntBinomialHeap *heap, int *out_key);
bool int_binomial_heap_extract_min(IntBinomialHeap *heap, int *out_key);
bool int_binomial_heap_meld(IntBinomialHeap *destination, IntBinomialHeap *source);

bool int_binomial_node_decrease_key(IntBinomialNode *node, int new_key);
bool int_binomial_heap_delete_handle(IntBinomialHeap *heap, IntBinomialNode *node);

int int_binomial_node_key(const IntBinomialNode *node);
bool int_binomial_heap_validate(const IntBinomialHeap *heap);

#endif
