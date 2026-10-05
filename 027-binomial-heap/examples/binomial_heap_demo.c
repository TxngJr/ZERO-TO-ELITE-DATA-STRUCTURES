#include "int_binomial_heap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBinomialHeap *a = int_binomial_heap_create();
    IntBinomialHeap *b = int_binomial_heap_create();

    assert(a != NULL && b != NULL);

    assert(int_binomial_heap_insert(a, 10) != NULL);
    assert(int_binomial_heap_insert(a, 3) != NULL);
    assert(int_binomial_heap_insert(b, 7) != NULL);
    assert(int_binomial_heap_insert(b, 1) != NULL);

    assert(int_binomial_heap_meld(a, b));
    assert(int_binomial_heap_size(b) == 0);
    assert(int_binomial_heap_validate(a));

    printf("melded pop order:");

    int value = 0;
    while (int_binomial_heap_extract_min(a, &value)) {
        printf(" %d", value);
    }
    putchar('\n');

    int_binomial_heap_free(a);
    int_binomial_heap_free(b);
    return 0;
}
