#include "int_cartesian_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void naive_min(
    const int64_t *values,
    size_t left,
    size_t right,
    size_t *out_index,
    int64_t *out_value
) {
    size_t best=left;

    for(size_t i=left+1;i<right;++i) {
        if(values[i]<values[best])best=i;
    }

    *out_index=best;
    *out_value=values[best];
}

static void test_basic(void) {
    const int64_t values[]={3,1,4,0,2};

    IntCartesianTree *tree=
        int_cartesian_tree_create(values,5);
    assert(tree!=NULL);
    assert(int_cartesian_tree_validate(tree));

    size_t root=0;

    assert(int_cartesian_tree_root(tree,&root));
    assert(root==3);

    size_t index=0;
    int64_t value=0;

    assert(int_cartesian_tree_range_min(
        tree,1,4,&index,&value
    ));
    assert(index==3&&value==0);

    assert(int_cartesian_tree_range_min(
        tree,0,3,&index,&value
    ));
    assert(index==1&&value==1);

    int_cartesian_tree_free(tree);
}

static void test_duplicates(void) {
    const int64_t values[]={5,2,2,2,7};

    IntCartesianTree *tree=
        int_cartesian_tree_create(values,5);
    assert(tree!=NULL);
    assert(int_cartesian_tree_validate(tree));

    size_t index=0;
    int64_t value=0;

    assert(int_cartesian_tree_range_min(
        tree,1,4,&index,&value
    ));

    assert(index==1);
    assert(value==2);

    int_cartesian_tree_free(tree);
}

static void test_shapes(void) {
    enum { N=256 };

    int64_t ascending[N];
    int64_t descending[N];

    for(size_t i=0;i<N;++i) {
        ascending[i]=(int64_t)i;
        descending[i]=(int64_t)(N-i);
    }

    IntCartesianTree *a=
        int_cartesian_tree_create(ascending,N);
    IntCartesianTree *d=
        int_cartesian_tree_create(descending,N);

    assert(a!=NULL&&d!=NULL);
    assert(int_cartesian_tree_validate(a));
    assert(int_cartesian_tree_validate(d));

    size_t height=0;

    assert(int_cartesian_tree_height(a,&height));
    assert(height==N-1);

    assert(int_cartesian_tree_height(d,&height));
    assert(height==N-1);

    int_cartesian_tree_free(a);
    int_cartesian_tree_free(d);
}

static void test_randomized(void) {
    enum { N=1000, QUERIES=50000 };

    int64_t *values=malloc(N*sizeof *values);
    assert(values!=NULL);

    uint32_t rng=0x53A12345u;

    for(size_t i=0;i<N;++i) {
        values[i]=(int64_t)(next_rng(&rng)%2001U)-1000;
    }

    IntCartesianTree *tree=
        int_cartesian_tree_create(values,N);
    assert(tree!=NULL);
    assert(int_cartesian_tree_validate(tree));

    for(int q=0;q<QUERIES;++q) {
        size_t left=next_rng(&rng)%N;
        size_t right=next_rng(&rng)%N;

        if(left>right) {
            const size_t tmp=left;
            left=right;
            right=tmp;
        }

        ++right;

        size_t expected_index=0;
        int64_t expected_value=0;

        naive_min(
            values,left,right,
            &expected_index,&expected_value
        );

        size_t actual_index=0;
        int64_t actual_value=0;

        assert(int_cartesian_tree_range_min(
            tree,left,right,
            &actual_index,&actual_value
        ));

        assert(actual_index==expected_index);
        assert(actual_value==expected_value);
    }

    int_cartesian_tree_free(tree);
    free(values);
}

int main(void) {
    IntCartesianTree *empty=
        int_cartesian_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(int_cartesian_tree_validate(empty));
    int_cartesian_tree_free(empty);

    test_basic();
    test_duplicates();
    test_shapes();
    test_randomized();

    puts("Cartesian Tree tests passed");
    return 0;
}
