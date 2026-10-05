#include "int_fenwick_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static int64_t naive_range(
    const int64_t *values,
    size_t left,
    size_t right
) {
    int64_t sum=0;

    for(size_t i=left;i<right;++i)sum+=values[i];

    return sum;
}

static void test_basic(void) {
    const int64_t values[]={1,2,3,4,5,6,7,8};

    IntFenwickTree *tree=
        int_fenwick_tree_create(values,8);
    assert(tree!=NULL);

    int64_t value=0;

    assert(int_fenwick_tree_prefix_sum(tree,0,&value));
    assert(value==0);

    assert(int_fenwick_tree_prefix_sum(tree,8,&value));
    assert(value==36);

    assert(int_fenwick_tree_range_sum(tree,2,6,&value));
    assert(value==18);

    assert(int_fenwick_tree_point_add(tree,3,10));
    assert(int_fenwick_tree_point_get(tree,3,&value));
    assert(value==14);

    assert(int_fenwick_tree_point_set(tree,3,-4));
    assert(int_fenwick_tree_point_get(tree,3,&value));
    assert(value==-4);

    assert(!int_fenwick_tree_range_sum(tree,4,4,&value));
    assert(!int_fenwick_tree_prefix_sum(tree,9,&value));
    assert(int_fenwick_tree_validate(tree));

    int_fenwick_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=513, STEPS=30000 };

    int64_t values[N];

    for(size_t i=0;i<N;++i) {
        values[i]=(int64_t)(i%37)-18;
    }

    IntFenwickTree *tree=
        int_fenwick_tree_create(values,N);
    assert(tree!=NULL);

    uint32_t rng=0x47A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%4U;

        if(op==0U) {
            const size_t index=next_rng(&rng)%N;
            const int64_t delta=
                (int64_t)(next_rng(&rng)%101U)-50;

            values[index]+=delta;

            assert(int_fenwick_tree_point_add(
                tree,index,delta
            ));
        } else if(op==1U) {
            const size_t index=next_rng(&rng)%N;
            const int64_t value=
                (int64_t)(next_rng(&rng)%2001U)-1000;

            values[index]=value;

            assert(int_fenwick_tree_point_set(
                tree,index,value
            ));
        } else if(op==2U) {
            const size_t end=next_rng(&rng)%(N+1U);
            int64_t actual=0;

            assert(int_fenwick_tree_prefix_sum(
                tree,end,&actual
            ));

            assert(actual==naive_range(values,0,end));
        } else {
            size_t left=next_rng(&rng)%N;
            size_t right=next_rng(&rng)%N;

            if(left>right) {
                const size_t tmp=left;
                left=right;
                right=tmp;
            }

            ++right;

            int64_t actual=0;

            assert(int_fenwick_tree_range_sum(
                tree,left,right,&actual
            ));

            assert(actual==naive_range(
                values,left,right
            ));
        }

        if((step%1009)==0) {
            const size_t index=next_rng(&rng)%N;
            int64_t actual=0;

            assert(int_fenwick_tree_point_get(
                tree,index,&actual
            ));
            assert(actual==values[index]);
        }
    }

    assert(int_fenwick_tree_validate(tree));
    int_fenwick_tree_free(tree);
}

int main(void) {
    IntFenwickTree *empty=
        int_fenwick_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(int_fenwick_tree_validate(empty));
    int_fenwick_tree_free(empty);

    test_basic();
    test_randomized();

    puts("Fenwick Tree tests passed");
    return 0;
}
