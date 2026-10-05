#include "heap_sort.h"
#include "int_min_heap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const int input[] = {9,4,7,1,3,6,2};

    IntMinHeap *heap = int_min_heap_build(
        input,
        sizeof input / sizeof input[0]
    );
    assert(heap != NULL);
    assert(int_min_heap_validate(heap));

    printf("heap pop order:");
    int value = 0;

    while (int_min_heap_pop(heap, &value)) {
        printf(" %d", value);
    }
    putchar('\n');

    int sortable[] = {5,1,9,3,7,2};
    int_heap_sort_ascending(sortable, 6);

    printf("sorted:");
    for (size_t i = 0; i < 6; ++i) printf(" %d", sortable[i]);
    putchar('\n');

    int_min_heap_free(heap);
    return 0;
}
