#include "dynamic_segment_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void test_huge_domain(void) {
    const uint64_t high=
        UINT64_C(1000000000000000000);

    DynamicSegmentTree *tree=
        dynamic_segment_tree_create(0,high);
    assert(tree!=NULL);

    assert(dynamic_segment_tree_point_add(tree,3,10));
    assert(dynamic_segment_tree_point_add(
        tree,high-1,7
    ));
    assert(dynamic_segment_tree_point_add(
        tree,UINT64_C(500000000000000000),-2
    ));

    int64_t sum=0;

    assert(dynamic_segment_tree_range_sum(
        tree,0,high,&sum
    ));
    assert(sum==15);

    assert(dynamic_segment_tree_point_get(tree,3,&sum));
    assert(sum==10);

    const size_t before=
        dynamic_segment_tree_node_count(tree);

    assert(dynamic_segment_tree_point_add(tree,3,-10));

    assert(dynamic_segment_tree_point_get(tree,3,&sum));
    assert(sum==0);

    assert(dynamic_segment_tree_node_count(tree)<before);
    assert(dynamic_segment_tree_validate(tree));

    dynamic_segment_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=2048, STEPS=20000 };

    int64_t reference[N];
    memset(reference,0,sizeof reference);

    DynamicSegmentTree *tree=
        dynamic_segment_tree_create(0,N);
    assert(tree!=NULL);

    uint32_t rng=0x45A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%3U;

        if(op<=1U) {
            const uint64_t index=next_rng(&rng)%N;
            const int64_t delta=
                (int64_t)(next_rng(&rng)%31U)-15;

            reference[index]+=delta;

            assert(dynamic_segment_tree_point_add(
                tree,index,delta
            ));
        } else {
            uint64_t left=next_rng(&rng)%N;
            uint64_t right=next_rng(&rng)%N;

            if(left>right) {
                const uint64_t tmp=left;
                left=right;
                right=tmp;
            }

            ++right;

            int64_t expected=0;

            for(uint64_t i=left;i<right;++i) {
                expected+=reference[i];
            }

            int64_t actual=0;

            assert(dynamic_segment_tree_range_sum(
                tree,left,right,&actual
            ));
            assert(actual==expected);
        }

        if((step%251)==0) {
            assert(dynamic_segment_tree_validate(tree));

            const uint64_t index=next_rng(&rng)%N;
            int64_t actual=0;

            assert(dynamic_segment_tree_point_get(
                tree,index,&actual
            ));
            assert(actual==reference[index]);
        }
    }

    assert(dynamic_segment_tree_validate(tree));
    dynamic_segment_tree_free(tree);
}

int main(void) {
    assert(dynamic_segment_tree_create(5,5)==NULL);

    test_huge_domain();
    test_randomized();

    puts("Dynamic Segment Tree tests passed");
    return 0;
}
