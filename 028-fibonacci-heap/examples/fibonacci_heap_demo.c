#include "int_fibonacci_heap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntFibonacciHeap *a = int_fib_heap_create();
    IntFibonacciHeap *b = int_fib_heap_create();
    assert(a != NULL && b != NULL);

    IntFibonacciNode *h10 = int_fib_heap_insert(a, 10);
    IntFibonacciNode *h30 = int_fib_heap_insert(a, 30);
    assert(h10 != NULL && h30 != NULL);

    assert(int_fib_heap_insert(b, 5) != NULL);
    assert(int_fib_heap_insert(b, 20) != NULL);

    assert(int_fib_heap_meld(a, b));
    assert(int_fib_heap_size(b) == 0);
    assert(int_fib_heap_validate(a));

    assert(int_fib_heap_decrease_key(a, h30, 1));

    int minimum = 0;
    assert(int_fib_heap_peek_min(a, &minimum));
    printf("minimum after decrease-key=%d\n", minimum);

    printf("extract order:");
    while (int_fib_heap_extract_min(a, &minimum)) {
        printf(" %d", minimum);
    }
    putchar('\n');

    int_fib_heap_free(a);
    int_fib_heap_free(b);
    (void)h10;
    return 0;
}
