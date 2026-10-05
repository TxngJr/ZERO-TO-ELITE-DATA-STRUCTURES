#include "int_segment_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void naive_query(
    const int64_t *a,
    size_t left,
    size_t right,
    int64_t *sum,
    int64_t *min
) {
    *sum=0;
    *min=a[left];

    for(size_t i=left;i<right;++i) {
        *sum+=a[i];
        if(a[i]<*min)*min=a[i];
    }
}

static void test_basic(void) {
    const int64_t values[]={5,2,7,1,6,3};

    IntSegmentTree *tree=int_segment_tree_create(values,6);
    assert(tree!=NULL);

    int64_t value=0;

    assert(int_segment_tree_range_sum(tree,0,6,&value));
    assert(value==24);

    assert(int_segment_tree_range_min(tree,0,6,&value));
    assert(value==1);

    assert(int_segment_tree_range_sum(tree,2,5,&value));
    assert(value==14);

    assert(int_segment_tree_point_set(tree,3,10));
    assert(int_segment_tree_point_get(tree,3,&value));
    assert(value==10);

    assert(int_segment_tree_range_min(tree,0,6,&value));
    assert(value==2);

    assert(!int_segment_tree_range_sum(tree,3,3,&value));
    assert(!int_segment_tree_range_sum(tree,0,7,&value));
    assert(!int_segment_tree_point_set(tree,6,1));

    assert(int_segment_tree_validate(tree));
    int_segment_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=257, STEPS=30000 };

    int64_t values[N];

    for(size_t i=0;i<N;++i) {
        values[i]=(int64_t)((int)i%31)-15;
    }

    IntSegmentTree *tree=int_segment_tree_create(values,N);
    assert(tree!=NULL);

    uint32_t rng=0x43A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const size_t index=next_rng(&rng)%N;
            const int64_t value=
                (int64_t)(next_rng(&rng)%2001U)-1000;

            values[index]=value;
            assert(int_segment_tree_point_set(
                tree,index,value
            ));
        } else {
            size_t a=next_rng(&rng)%N;
            size_t b=next_rng(&rng)%N;

            if(a>b) {
                const size_t tmp=a;a=b;b=tmp;
            }

            if(a==b) {
                b=(b+1)%N;

                if(a>b) {
                    const size_t tmp=a;a=b;b=tmp;
                }

                if(a==b) {
                    a=0;
                    b=N;
                }
            }

            ++b;
            if(b>N)b=N;

            int64_t expected_sum=0;
            int64_t expected_min=0;
            naive_query(
                values,a,b,&expected_sum,&expected_min
            );

            int64_t actual=0;

            if(op==1U) {
                assert(int_segment_tree_range_sum(
                    tree,a,b,&actual
                ));
                assert(actual==expected_sum);
            } else {
                assert(int_segment_tree_range_min(
                    tree,a,b,&actual
                ));
                assert(actual==expected_min);
            }
        }

        if((step%257)==0) {
            assert(int_segment_tree_validate(tree));
        }
    }

    assert(int_segment_tree_validate(tree));
    int_segment_tree_free(tree);
}

int main(void) {
    IntSegmentTree *empty=int_segment_tree_create(NULL,0);
    assert(empty!=NULL);
    assert(int_segment_tree_validate(empty));
    int_segment_tree_free(empty);

    test_basic();
    test_randomized();

    puts("Segment Tree tests passed");
    return 0;
}
