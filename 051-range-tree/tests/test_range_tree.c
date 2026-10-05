#include "int_range_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool inside(
    const IntPoint2D *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh;
}

static size_t naive_count(
    const IntPoint2D *points,
    size_t count,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    size_t result=0;

    for(size_t i=0;i<count;++i) {
        if(inside(&points[i],xl,xh,yl,yh))++result;
    }

    return result;
}

static bool id_in_naive_result(
    const IntPoint2D *points,
    size_t count,
    uint64_t id,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    for(size_t i=0;i<count;++i) {
        if(points[i].id==id&&
           inside(&points[i],xl,xh,yl,yh)) {
            return true;
        }
    }

    return false;
}

static void test_basic(void) {
    const IntPoint2D points[]={
        {1,5,1},{2,9,2},{4,3,3},{5,12,4},
        {7,7,5},{8,15,6},{9,4,7},
        {5,12,8}
    };

    const size_t n=sizeof points/sizeof points[0];

    IntRangeTree *tree=int_range_tree_create(points,n);
    assert(tree!=NULL);
    assert(int_range_tree_validate(tree));

    size_t count=0;
    assert(int_range_tree_query_count(
        tree,2,9,4,13,&count
    ));

    const size_t expected=naive_count(
        points,n,2,9,4,13
    );

    assert(count==expected);

    IntPoint2D output[16];
    size_t written=0;

    assert(int_range_tree_query_report(
        tree,2,9,4,13,
        output,16,&written
    ));

    assert(written==expected);

    for(size_t i=0;i<written;++i) {
        assert(id_in_naive_result(
            points,n,output[i].id,
            2,9,4,13
        ));
    }

    assert(!int_range_tree_query_count(
        tree,5,5,0,10,&count
    ));

    int_range_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=1000, QUERIES=30000 };

    IntPoint2D *points=malloc(N*sizeof *points);
    assert(points!=NULL);

    uint32_t rng=0x51A12345u;

    for(size_t i=0;i<N;++i) {
        points[i]=(IntPoint2D){
            .x=(int64_t)(next_rng(&rng)%401U)-200,
            .y=(int64_t)(next_rng(&rng)%401U)-200,
            .id=(uint64_t)i+1
        };
    }

    IntRangeTree *tree=int_range_tree_create(points,N);
    assert(tree!=NULL);
    assert(int_range_tree_validate(tree));

    IntPoint2D *output=malloc(N*sizeof *output);
    assert(output!=NULL);

    for(int q=0;q<QUERIES;++q) {
        int64_t xl=(int64_t)(next_rng(&rng)%451U)-225;
        int64_t xh=(int64_t)(next_rng(&rng)%451U)-225;
        int64_t yl=(int64_t)(next_rng(&rng)%451U)-225;
        int64_t yh=(int64_t)(next_rng(&rng)%451U)-225;

        if(xl>xh) {
            const int64_t t=xl;xl=xh;xh=t;
        }

        if(yl>yh) {
            const int64_t t=yl;yl=yh;yh=t;
        }

        if(xl==xh)++xh;
        if(yl==yh)++yh;

        const size_t expected=naive_count(
            points,N,xl,xh,yl,yh
        );

        size_t actual=0;

        assert(int_range_tree_query_count(
            tree,xl,xh,yl,yh,&actual
        ));
        assert(actual==expected);

        if((q%101)==0) {
            size_t written=0;

            assert(int_range_tree_query_report(
                tree,xl,xh,yl,yh,
                output,N,&written
            ));

            assert(written==expected);

            for(size_t i=0;i<written;++i) {
                assert(inside(
                    &output[i],
                    xl,xh,yl,yh
                ));
                assert(output[i].id>=1&&output[i].id<=N);
            }
        }
    }

    free(output);
    int_range_tree_free(tree);
    free(points);
}

static void test_sizes(void) {
    for(size_t n=1;n<=129;++n) {
        IntPoint2D *points=malloc(n*sizeof *points);
        assert(points!=NULL);

        for(size_t i=0;i<n;++i) {
            points[i]=(IntPoint2D){
                .x=(int64_t)(i%7),
                .y=(int64_t)((i*13U)%17U),
                .id=(uint64_t)i
            };
        }

        IntRangeTree *tree=int_range_tree_create(points,n);
        assert(tree!=NULL);
        assert(int_range_tree_validate(tree));

        int_range_tree_free(tree);
        free(points);
    }
}

int main(void) {
    IntRangeTree *empty=int_range_tree_create(NULL,0);
    assert(empty!=NULL);
    assert(int_range_tree_validate(empty));
    int_range_tree_free(empty);

    test_basic();
    test_randomized();
    test_sizes();

    puts("Range Tree tests passed");
    return 0;
}
