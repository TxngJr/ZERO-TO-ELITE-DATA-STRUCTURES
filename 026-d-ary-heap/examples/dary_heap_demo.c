#include "int_dary_heap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const int values[] = {9,4,7,1,3,6,2,8,5};

    IntDaryHeap *heap = int_dary_heap_build(4, values, 9);
    assert(heap != NULL);
    assert(int_dary_heap_validate(heap));

    printf("d=%zu pop order:", int_dary_heap_arity(heap));

    int value = 0;
    while (int_dary_heap_pop(heap, &value)) {
        printf(" %d", value);
    }
    putchar('\n');

    int_dary_heap_free(heap);
    return 0;
}
