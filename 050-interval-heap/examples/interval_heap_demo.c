#include "int_interval_heap.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    IntIntervalHeap *heap=int_interval_heap_create();
    assert(heap!=NULL);

    const int64_t values[]={30,5,80,20,90,2,70,40,10};

    for(size_t i=0;i<sizeof values/sizeof values[0];++i) {
        assert(int_interval_heap_insert(heap,values[i]));
    }

    int64_t min=0;
    int64_t max=0;

    assert(int_interval_heap_peek_min(heap,&min));
    assert(int_interval_heap_peek_max(heap,&max));

    printf("min=%" PRId64 " max=%" PRId64 "\n",min,max);

    assert(int_interval_heap_pop_min(heap,&min));
    assert(int_interval_heap_pop_max(heap,&max));

    printf("popped min=%" PRId64 " max=%" PRId64 "\n",min,max);

    assert(int_interval_heap_validate(heap));
    int_interval_heap_free(heap);
    return 0;
}
