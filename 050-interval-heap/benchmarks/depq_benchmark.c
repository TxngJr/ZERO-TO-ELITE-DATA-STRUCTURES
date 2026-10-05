#include "int_interval_heap.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t operations=500000;
    IntIntervalHeap *heap=int_interval_heap_create();

    if(heap==NULL)return 1;

    uint32_t rng=123456789u;
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);

    for(size_t i=0;i<operations;++i) {
        rng=rng*1664525u+1013904223u;

        if(int_interval_heap_size(heap)<64 ||
           (rng%3U)!=0U) {
            if(!int_interval_heap_insert(
                    heap,(int64_t)rng
                )) {
                return 1;
            }
        } else {
            int64_t value=0;

            if((rng&1U)==0U) {
                if(!int_interval_heap_pop_min(
                        heap,&value
                    )) {
                    return 1;
                }
            } else {
                if(!int_interval_heap_pop_max(
                        heap,&value
                    )) {
                    return 1;
                }
            }
        }
    }

    timespec_get(&b,TIME_UTC);

    printf(
        "operations=%zu remaining=%zu seconds=%.9f\n",
        operations,
        int_interval_heap_size(heap),
        elapsed(a,b)
    );

    int_interval_heap_free(heap);
    return 0;
}
