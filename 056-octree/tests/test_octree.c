#include "int_octree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool inside(
    const IntOctPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh&&
           zl<=p->z&&p->z<zh;
}

static size_t naive_count(
    const IntOctPoint *points,
    size_t count,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    size_t result=0;

    for(size_t i=0;i<count;++i) {
        if(inside(&points[i],xl,xh,yl,yh,zl,zh))++result;
    }

    return result;
}

static void test_basic(void) {
    IntOctree *tree=int_octree_create(
        -64,64,-64,64,-64,64,2,16
    );
    assert(tree!=NULL);

    for(size_t i=0;i<8;++i) {
        const int64_t x=(i&1U)?20:-20;
        const int64_t y=(i&2U)?20:-20;
        const int64_t z=(i&4U)?20:-20;

        assert(int_octree_insert(
            tree,(IntOctPoint){x,y,z,(uint64_t)i}
        ));
    }

    size_t count=0;

    assert(int_octree_query_count(
        tree,0,64,0,64,0,64,&count
    ));
    assert(count==1);

    assert(int_octree_validate(tree));
    int_octree_free(tree);
}

static void test_coincident(void) {
    IntOctree *tree=int_octree_create(
        0,1024,0,1024,0,1024,1,6
    );
    assert(tree!=NULL);

    for(uint64_t id=0;id<80;++id) {
        assert(int_octree_insert(
            tree,(IntOctPoint){512,512,512,id}
        ));
    }

    size_t count=0;

    assert(int_octree_query_count(
        tree,512,513,512,513,512,513,&count
    ));
    assert(count==80);

    assert(int_octree_validate(tree));
    int_octree_free(tree);
}

static void test_randomized(void) {
    enum { N=3000, QUERIES=8000 };

    IntOctPoint *points=malloc(N*sizeof *points);
    IntOctPoint *output=malloc(N*sizeof *output);

    assert(points!=NULL&&output!=NULL);

    IntOctree *tree=int_octree_create(
        -1024,1024,-1024,1024,-1024,1024,8,18
    );
    assert(tree!=NULL);

    uint32_t rng=0x56A12345u;

    for(size_t i=0;i<N;++i) {
        points[i]=(IntOctPoint){
            .x=(int64_t)(next_rng(&rng)%2048U)-1024,
            .y=(int64_t)(next_rng(&rng)%2048U)-1024,
            .z=(int64_t)(next_rng(&rng)%2048U)-1024,
            .id=(uint64_t)i+1
        };

        assert(int_octree_insert(tree,points[i]));
    }

    assert(int_octree_validate(tree));

    for(int q=0;q<QUERIES;++q) {
        int64_t xl=(int64_t)(next_rng(&rng)%2200U)-1100;
        int64_t xh=(int64_t)(next_rng(&rng)%2200U)-1100;
        int64_t yl=(int64_t)(next_rng(&rng)%2200U)-1100;
        int64_t yh=(int64_t)(next_rng(&rng)%2200U)-1100;
        int64_t zl=(int64_t)(next_rng(&rng)%2200U)-1100;
        int64_t zh=(int64_t)(next_rng(&rng)%2200U)-1100;

        if(xl>xh){const int64_t t=xl;xl=xh;xh=t;}
        if(yl>yh){const int64_t t=yl;yl=yh;yh=t;}
        if(zl>zh){const int64_t t=zl;zl=zh;zh=t;}

        if(xl==xh)++xh;
        if(yl==yh)++yh;
        if(zl==zh)++zh;

        const size_t expected=naive_count(
            points,N,xl,xh,yl,yh,zl,zh
        );

        size_t actual=0;

        assert(int_octree_query_count(
            tree,xl,xh,yl,yh,zl,zh,&actual
        ));
        assert(actual==expected);

        if((q%173)==0) {
            size_t written=0;

            assert(int_octree_query_report(
                tree,xl,xh,yl,yh,zl,zh,
                output,N,&written
            ));

            assert(written==expected);

            for(size_t i=0;i<written;++i) {
                assert(inside(
                    &output[i],xl,xh,yl,yh,zl,zh
                ));
            }
        }
    }

    int_octree_free(tree);
    free(output);
    free(points);
}

int main(void) {
    assert(int_octree_create(
        0,1,0,1,0,0,4,8
    )==NULL);

    test_basic();
    test_coincident();
    test_randomized();

    puts("Octree tests passed");
    return 0;
}
