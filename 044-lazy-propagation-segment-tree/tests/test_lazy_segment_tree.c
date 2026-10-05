#include "lazy_segment_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void ordered_range(
    uint32_t *rng,
    size_t n,
    size_t *left,
    size_t *right
) {
    size_t a=next_rng(rng)%n;
    size_t b=next_rng(rng)%n;

    if(a>b) {
        const size_t tmp=a;a=b;b=tmp;
    }

    *left=a;
    *right=b+1;
}

static void naive_query(
    const int64_t *values,
    size_t left,
    size_t right,
    int64_t *sum,
    int64_t *min
) {
    *sum=0;
    *min=values[left];

    for(size_t i=left;i<right;++i) {
        *sum+=values[i];
        if(values[i]<*min)*min=values[i];
    }
}

static void test_basic(void) {
    int64_t values[]={1,2,3,4,5};

    LazySegmentTree *tree=
        lazy_segment_tree_create(values,5);
    assert(tree!=NULL);

    assert(lazy_segment_tree_range_add(tree,1,4,10));

    int64_t value=0;

    assert(lazy_segment_tree_range_sum(tree,0,5,&value));
    assert(value==45);

    assert(lazy_segment_tree_range_min(tree,0,5,&value));
    assert(value==1);

    assert(lazy_segment_tree_range_min(tree,1,4,&value));
    assert(value==12);

    assert(lazy_segment_tree_range_add(tree,0,5,-3));

    assert(lazy_segment_tree_point_get(tree,2,&value));
    assert(value==10);

    assert(lazy_segment_tree_validate(tree));
    lazy_segment_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=193, STEPS=15000 };

    int64_t values[N];

    for(size_t i=0;i<N;++i) {
        values[i]=(int64_t)(i%23)-11;
    }

    LazySegmentTree *tree=
        lazy_segment_tree_create(values,N);
    assert(tree!=NULL);

    uint32_t rng=0x44A12345u;

    for(int step=0;step<STEPS;++step) {
        size_t left=0;
        size_t right=0;
        ordered_range(&rng,N,&left,&right);

        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const int64_t delta=
                (int64_t)(next_rng(&rng)%41U)-20;

            for(size_t i=left;i<right;++i) {
                values[i]+=delta;
            }

            assert(lazy_segment_tree_range_add(
                tree,left,right,delta
            ));
        } else {
            int64_t expected_sum=0;
            int64_t expected_min=0;

            naive_query(
                values,left,right,
                &expected_sum,&expected_min
            );

            int64_t actual=0;

            if(op==1U) {
                assert(lazy_segment_tree_range_sum(
                    tree,left,right,&actual
                ));
                assert(actual==expected_sum);
            } else {
                assert(lazy_segment_tree_range_min(
                    tree,left,right,&actual
                ));
                assert(actual==expected_min);
            }
        }

        if((step%211)==0) {
            assert(lazy_segment_tree_validate(tree));

            const size_t index=next_rng(&rng)%N;
            int64_t actual=0;
            assert(lazy_segment_tree_point_get(
                tree,index,&actual
            ));
            assert(actual==values[index]);
        }
    }

    assert(lazy_segment_tree_validate(tree));
    lazy_segment_tree_free(tree);
}

int main(void) {
    LazySegmentTree *empty=
        lazy_segment_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(lazy_segment_tree_validate(empty));
    lazy_segment_tree_free(empty);

    test_basic();
    test_randomized();

    puts("Lazy Segment Tree tests passed");
    return 0;
}
