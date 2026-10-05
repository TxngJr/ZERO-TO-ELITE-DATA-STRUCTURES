#include "int_r_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool overlap(
    const IntRRect *r,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return r->x_low<xh&&xl<r->x_high&&
           r->y_low<yh&&yl<r->y_high;
}

static size_t naive_count(
    const IntRRect *rects,
    size_t count,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    size_t result=0;

    for(size_t i=0;i<count;++i) {
        if(overlap(&rects[i],xl,xh,yl,yh))++result;
    }

    return result;
}

static void test_basic(void) {
    IntRTree *tree=int_r_tree_create();
    assert(tree!=NULL);
    assert(int_r_tree_validate(tree));

    const IntRRect rects[]={
        {0,10,0,10,1},
        {20,30,20,30,2},
        {5,15,5,15,3},
        {40,50,0,10,4},
        {22,28,10,25,5},
        {-10,-2,-10,-2,6},
        {60,80,60,80,7}
    };

    for(size_t i=0;i<7;++i) {
        assert(int_r_tree_insert(tree,rects[i]));
        assert(int_r_tree_validate(tree));
    }

    size_t count=0;

    assert(int_r_tree_query_count(
        tree,8,24,8,24,&count
    ));

    assert(count==4);

    IntRRect output[8];
    size_t written=0;

    assert(int_r_tree_query_report(
        tree,8,24,8,24,
        output,8,&written
    ));

    assert(written==count);

    assert(!int_r_tree_insert(
        tree,(IntRRect){1,1,0,2,8}
    ));

    int_r_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=5000, QUERIES=12000 };

    IntRRect *rects=malloc(N*sizeof *rects);
    IntRRect *output=malloc(N*sizeof *output);

    assert(rects!=NULL&&output!=NULL);

    IntRTree *tree=int_r_tree_create();
    assert(tree!=NULL);

    uint32_t rng=0x57A12345u;

    for(size_t i=0;i<N;++i) {
        const int64_t xl=
            (int64_t)(next_rng(&rng)%4001U)-2000;
        const int64_t yl=
            (int64_t)(next_rng(&rng)%4001U)-2000;

        const int64_t w=
            (int64_t)(next_rng(&rng)%80U)+1;
        const int64_t h=
            (int64_t)(next_rng(&rng)%80U)+1;

        rects[i]=(IntRRect){
            .x_low=xl,
            .x_high=xl+w,
            .y_low=yl,
            .y_high=yl+h,
            .id=(uint64_t)i+1
        };

        assert(int_r_tree_insert(tree,rects[i]));
    }

    assert(int_r_tree_validate(tree));

    for(int q=0;q<QUERIES;++q) {
        int64_t xl=(int64_t)(next_rng(&rng)%4201U)-2100;
        int64_t yl=(int64_t)(next_rng(&rng)%4201U)-2100;
        int64_t xh=xl+(int64_t)(next_rng(&rng)%300U)+1;
        int64_t yh=yl+(int64_t)(next_rng(&rng)%300U)+1;

        const size_t expected=naive_count(
            rects,N,xl,xh,yl,yh
        );

        size_t actual=0;

        assert(int_r_tree_query_count(
            tree,xl,xh,yl,yh,&actual
        ));
        assert(actual==expected);

        if((q%157)==0) {
            size_t written=0;

            assert(int_r_tree_query_report(
                tree,xl,xh,yl,yh,
                output,N,&written
            ));

            assert(written==expected);

            for(size_t i=0;i<written;++i) {
                assert(overlap(
                    &output[i],xl,xh,yl,yh
                ));
            }
        }
    }

    int_r_tree_free(tree);
    free(output);
    free(rects);
}

int main(void) {
    test_basic();
    test_randomized();
    puts("R-Tree tests passed");
    return 0;
}
