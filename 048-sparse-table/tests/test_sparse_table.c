#include "int_sparse_table.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static int64_t naive_min(
    const int64_t *values,
    size_t left,
    size_t right
) {
    int64_t result=values[left];

    for(size_t i=left+1;i<right;++i) {
        if(values[i]<result)result=values[i];
    }

    return result;
}

static void test_basic(void) {
    const int64_t values[]={8,3,6,1,7,2,5,4};

    IntSparseTable *table=
        int_sparse_table_create(values,8);
    assert(table!=NULL);

    assert(int_sparse_table_levels(table)==4);

    int64_t value=0;

    assert(int_sparse_table_range_min(
        table,0,8,&value
    ));
    assert(value==1);

    assert(int_sparse_table_range_min(
        table,2,7,&value
    ));
    assert(value==1);

    assert(int_sparse_table_range_min(
        table,4,8,&value
    ));
    assert(value==2);

    assert(int_sparse_table_range_min(
        table,6,7,&value
    ));
    assert(value==5);

    assert(!int_sparse_table_range_min(
        table,3,3,&value
    ));
    assert(!int_sparse_table_range_min(
        table,0,9,&value
    ));

    assert(int_sparse_table_validate(table));
    int_sparse_table_free(table);
}

static void test_randomized(void) {
    enum { N=1003, QUERIES=50000 };

    int64_t *values=malloc(N*sizeof *values);
    assert(values!=NULL);

    uint32_t rng=0x48A12345u;

    for(size_t i=0;i<N;++i) {
        rng=next_rng(&rng);
        values[i]=(int64_t)(rng%2000001U)-1000000;
    }

    IntSparseTable *table=
        int_sparse_table_create(values,N);
    assert(table!=NULL);
    assert(int_sparse_table_validate(table));

    for(int query=0;query<QUERIES;++query) {
        size_t left=next_rng(&rng)%N;
        size_t right=next_rng(&rng)%N;

        if(left>right) {
            const size_t tmp=left;
            left=right;
            right=tmp;
        }

        ++right;

        int64_t actual=0;

        assert(int_sparse_table_range_min(
            table,left,right,&actual
        ));

        assert(actual==naive_min(
            values,left,right
        ));
    }

    int_sparse_table_free(table);
    free(values);
}

static void test_non_power_of_two_sizes(void) {
    for(size_t n=1;n<=129;++n) {
        int64_t *values=malloc(n*sizeof *values);
        assert(values!=NULL);

        for(size_t i=0;i<n;++i) {
            values[i]=(int64_t)((i*37U)%101U)-50;
        }

        IntSparseTable *table=
            int_sparse_table_create(values,n);
        assert(table!=NULL);
        assert(int_sparse_table_validate(table));

        for(size_t left=0;left<n;++left) {
            const size_t right=
                left+1+(n-left-1)/2;

            int64_t actual=0;

            assert(int_sparse_table_range_min(
                table,left,right,&actual
            ));

            assert(actual==naive_min(
                values,left,right
            ));
        }

        int_sparse_table_free(table);
        free(values);
    }
}

int main(void) {
    IntSparseTable *empty=
        int_sparse_table_create(NULL,0);

    assert(empty!=NULL);
    assert(int_sparse_table_validate(empty));
    int_sparse_table_free(empty);

    test_basic();
    test_randomized();
    test_non_power_of_two_sizes();

    puts("Sparse Table tests passed");
    return 0;
}
