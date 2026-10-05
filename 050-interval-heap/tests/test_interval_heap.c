#include "int_interval_heap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int64_t *values;
    size_t count;
    size_t capacity;
} RefBag;

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool ref_push(RefBag *bag,int64_t value) {
    if(bag->count==bag->capacity) {
        size_t capacity=bag->capacity==0?16:bag->capacity*2;
        int64_t *next=realloc(
            bag->values,
            capacity*sizeof *next
        );
        if(next==NULL)return false;
        bag->values=next;
        bag->capacity=capacity;
    }

    bag->values[bag->count++]=value;
    return true;
}

static size_t ref_min_index(const RefBag *bag) {
    size_t index=0;

    for(size_t i=1;i<bag->count;++i) {
        if(bag->values[i]<bag->values[index])index=i;
    }

    return index;
}

static size_t ref_max_index(const RefBag *bag) {
    size_t index=0;

    for(size_t i=1;i<bag->count;++i) {
        if(bag->values[i]>bag->values[index])index=i;
    }

    return index;
}

static int64_t ref_remove_at(RefBag *bag,size_t index) {
    const int64_t value=bag->values[index];
    bag->values[index]=bag->values[bag->count-1];
    --bag->count;
    return value;
}

static void assert_extremes(
    const IntIntervalHeap *heap,
    const RefBag *bag
) {
    assert(int_interval_heap_size(heap)==bag->count);
    assert(int_interval_heap_validate(heap));

    int64_t actual=0;

    if(bag->count==0) {
        assert(!int_interval_heap_peek_min(heap,&actual));
        assert(!int_interval_heap_peek_max(heap,&actual));
        return;
    }

    assert(int_interval_heap_peek_min(heap,&actual));
    assert(actual==bag->values[ref_min_index(bag)]);

    assert(int_interval_heap_peek_max(heap,&actual));
    assert(actual==bag->values[ref_max_index(bag)]);
}

static void test_basic(void) {
    IntIntervalHeap *heap=int_interval_heap_create();
    assert(heap!=NULL);

    const int64_t values[]={5,1,9,3,7,1,9,4,6};

    for(size_t i=0;i<sizeof values/sizeof values[0];++i) {
        assert(int_interval_heap_insert(heap,values[i]));
        assert(int_interval_heap_validate(heap));
    }

    int64_t value=0;

    assert(int_interval_heap_peek_min(heap,&value));
    assert(value==1);

    assert(int_interval_heap_peek_max(heap,&value));
    assert(value==9);

    int64_t previous=INT64_MIN;

    while(!int_interval_heap_is_empty(heap)) {
        assert(int_interval_heap_pop_min(heap,&value));
        assert(value>=previous);
        previous=value;
        assert(int_interval_heap_validate(heap));
    }

    int_interval_heap_free(heap);
}

static void test_descending_max(void) {
    IntIntervalHeap *heap=int_interval_heap_create();
    assert(heap!=NULL);

    for(int64_t value=-50;value<=50;++value) {
        assert(int_interval_heap_insert(heap,value));
    }

    int64_t previous=INT64_MAX;
    int64_t value=0;

    while(!int_interval_heap_is_empty(heap)) {
        assert(int_interval_heap_pop_max(heap,&value));
        assert(value<=previous);
        previous=value;
        assert(int_interval_heap_validate(heap));
    }

    int_interval_heap_free(heap);
}

static void test_randomized(void) {
    enum { STEPS=40000 };

    IntIntervalHeap *heap=int_interval_heap_create();
    assert(heap!=NULL);

    RefBag bag={0};
    uint32_t rng=0x50A12345u;

    for(int step=0;step<STEPS;++step) {
        unsigned op=next_rng(&rng)%5U;

        if(bag.count==0)op=0;

        if(op<=2U) {
            const int64_t value=
                (int64_t)(next_rng(&rng)%2001U)-1000;

            assert(int_interval_heap_insert(heap,value));
            assert(ref_push(&bag,value));
        } else if(op==3U) {
            const size_t index=ref_min_index(&bag);
            const int64_t expected=ref_remove_at(&bag,index);
            int64_t actual=0;

            assert(int_interval_heap_pop_min(heap,&actual));
            assert(actual==expected);
        } else {
            const size_t index=ref_max_index(&bag);
            const int64_t expected=ref_remove_at(&bag,index);
            int64_t actual=0;

            assert(int_interval_heap_pop_max(heap,&actual));
            assert(actual==expected);
        }

        if((step%53)==0) {
            assert_extremes(heap,&bag);
        }
    }

    assert_extremes(heap,&bag);

    while(bag.count>0) {
        const bool take_min=(next_rng(&rng)&1U)==0U;
        int64_t expected=0;
        int64_t actual=0;

        if(take_min) {
            expected=ref_remove_at(
                &bag,ref_min_index(&bag)
            );
            assert(int_interval_heap_pop_min(
                heap,&actual
            ));
        } else {
            expected=ref_remove_at(
                &bag,ref_max_index(&bag)
            );
            assert(int_interval_heap_pop_max(
                heap,&actual
            ));
        }

        assert(actual==expected);
    }

    assert(int_interval_heap_is_empty(heap));
    assert(int_interval_heap_validate(heap));

    free(bag.values);
    int_interval_heap_free(heap);
}

int main(void) {
    test_basic();
    test_descending_max();
    test_randomized();

    puts("Interval Heap tests passed");
    return 0;
}
