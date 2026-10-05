#include "int_spatial_hash.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool inside(
    const IntSpatialPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&yl<=p->y&&p->y<yh;
}

static size_t naive_count(
    const IntSpatialPoint *points,size_t n,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    size_t count=0;

    for(size_t i=0;i<n;++i) {
        if(inside(&points[i],xl,xh,yl,yh))++count;
    }

    return count;
}

static void test_negative_cells(void) {
    IntSpatialHash *hash=int_spatial_hash_create(10);
    assert(hash!=NULL);

    const IntSpatialPoint points[]={
        {-1,-1,1},{-10,-10,2},{-11,-11,3},
        {0,0,4},{9,9,5},{10,10,6}
    };

    for(size_t i=0;i<6;++i) {
        assert(int_spatial_hash_insert(hash,points[i]));
    }

    assert(int_spatial_hash_cell_count(hash)==4);
    assert(int_spatial_hash_validate(hash));

    size_t count=0;

    assert(int_spatial_hash_query_count(
        hash,-10,0,-10,0,&count
    ));
    assert(count==2);

    int_spatial_hash_free(hash);
}

static void test_randomized(void) {
    enum { N=6000, QUERIES=15000 };

    IntSpatialPoint *points=malloc(N*sizeof *points);
    IntSpatialPoint *output=malloc(N*sizeof *output);

    assert(points!=NULL&&output!=NULL);

    IntSpatialHash *hash=int_spatial_hash_create(32);
    assert(hash!=NULL);

    uint32_t rng=0x58A12345u;

    for(size_t i=0;i<N;++i) {
        points[i]=(IntSpatialPoint){
            .x=(int64_t)(next_rng(&rng)%10001U)-5000,
            .y=(int64_t)(next_rng(&rng)%10001U)-5000,
            .id=(uint64_t)i+1
        };

        assert(int_spatial_hash_insert(hash,points[i]));
    }

    assert(int_spatial_hash_validate(hash));

    for(int q=0;q<QUERIES;++q) {
        int64_t xl=(int64_t)(next_rng(&rng)%10401U)-5200;
        int64_t yl=(int64_t)(next_rng(&rng)%10401U)-5200;
        const int64_t xh=xl+(int64_t)(next_rng(&rng)%300U)+1;
        const int64_t yh=yl+(int64_t)(next_rng(&rng)%300U)+1;

        const size_t expected=naive_count(
            points,N,xl,xh,yl,yh
        );

        size_t actual=0;

        assert(int_spatial_hash_query_count(
            hash,xl,xh,yl,yh,&actual
        ));
        assert(actual==expected);

        if((q%173)==0) {
            size_t written=0;

            assert(int_spatial_hash_query_report(
                hash,xl,xh,yl,yh,
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

    free(output);
    free(points);
    int_spatial_hash_free(hash);
}

int main(void) {
    assert(int_spatial_hash_create(0)==NULL);

    test_negative_cells();
    test_randomized();

    puts("Spatial Hash tests passed");
    return 0;
}
