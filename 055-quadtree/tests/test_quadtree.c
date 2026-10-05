#include "int_quadtree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool inside(
    const IntQuadPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh;
}

static size_t naive_count(
    const IntQuadPoint *points,
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

static void test_basic(void) {
    IntQuadtree *tree=int_quadtree_create(
        -100,100,-100,100,2,16
    );
    assert(tree!=NULL);

    const IntQuadPoint points[]={
        {-20,-10,1},{10,10,2},{40,40,3},
        {-50,60,4},{10,10,5},{99,99,6}
    };

    for(size_t i=0;i<6;++i) {
        assert(int_quadtree_insert(tree,points[i]));
        assert(int_quadtree_validate(tree));
    }

    size_t count=0;

    assert(int_quadtree_query_count(
        tree,0,50,0,50,&count
    ));
    assert(count==3);

    assert(!int_quadtree_insert(
        tree,(IntQuadPoint){100,0,7}
    ));

    IntQuadPoint output[8];
    size_t written=0;

    assert(int_quadtree_query_report(
        tree,-100,100,-100,100,
        output,8,&written
    ));
    assert(written==6);

    int_quadtree_free(tree);
}

static void test_coincident(void) {
    IntQuadtree *tree=int_quadtree_create(
        0,1024,0,1024,1,6
    );
    assert(tree!=NULL);

    for(uint64_t id=0;id<100;++id) {
        assert(int_quadtree_insert(
            tree,(IntQuadPoint){512,512,id}
        ));
    }

    size_t count=0;

    assert(int_quadtree_query_count(
        tree,512,513,512,513,&count
    ));

    assert(count==100);
    assert(int_quadtree_validate(tree));

    int_quadtree_free(tree);
}

static void test_randomized(void) {
    enum { N=5000, QUERIES=12000 };

    IntQuadPoint *points=malloc(N*sizeof *points);
    IntQuadPoint *output=malloc(N*sizeof *output);

    assert(points!=NULL&&output!=NULL);

    IntQuadtree *tree=int_quadtree_create(
        -2048,2048,-2048,2048,8,20
    );
    assert(tree!=NULL);

    uint32_t rng=0x55A12345u;

    for(size_t i=0;i<N;++i) {
        points[i]=(IntQuadPoint){
            .x=(int64_t)(next_rng(&rng)%4096U)-2048,
            .y=(int64_t)(next_rng(&rng)%4096U)-2048,
            .id=(uint64_t)i+1
        };

        assert(int_quadtree_insert(tree,points[i]));
    }

    assert(int_quadtree_validate(tree));

    for(int q=0;q<QUERIES;++q) {
        int64_t xl=(int64_t)(next_rng(&rng)%4400U)-2200;
        int64_t xh=(int64_t)(next_rng(&rng)%4400U)-2200;
        int64_t yl=(int64_t)(next_rng(&rng)%4400U)-2200;
        int64_t yh=(int64_t)(next_rng(&rng)%4400U)-2200;

        if(xl>xh) {
            const int64_t t=xl;xl=xh;xh=t;
        }

        if(yl>yh) {
            const int64_t t=yl;yl=yh;yh=t;
        }

        if(xl==xh)++xh;
        if(yl==yh)++yh;

        const size_t expected=
            naive_count(points,N,xl,xh,yl,yh);

        size_t actual=0;

        assert(int_quadtree_query_count(
            tree,xl,xh,yl,yh,&actual
        ));
        assert(actual==expected);

        if((q%149)==0) {
            size_t written=0;

            assert(int_quadtree_query_report(
                tree,xl,xh,yl,yh,
                output,N,&written
            ));

            assert(written==expected);

            for(size_t i=0;i<written;++i) {
                assert(inside(
                    &output[i],xl,xh,yl,yh
                ));
            }
        }
    }

    int_quadtree_free(tree);
    free(output);
    free(points);
}

int main(void) {
    assert(int_quadtree_create(
        0,0,0,1,4,10
    )==NULL);

    test_basic();
    test_coincident();
    test_randomized();

    puts("Quadtree tests passed");
    return 0;
}
