#include "int_kd_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool inside(
    const IntKDPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh;
}

static size_t naive_count(
    const IntKDPoint *points,
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

static long double distance_sq(
    const IntKDPoint *p,
    int64_t qx,
    int64_t qy
) {
    const long double dx=
        (long double)p->x-(long double)qx;
    const long double dy=
        (long double)p->y-(long double)qy;

    return dx*dx+dy*dy;
}

static bool point_less(
    const IntKDPoint *a,
    const IntKDPoint *b
) {
    if(a->x!=b->x)return a->x<b->x;
    if(a->y!=b->y)return a->y<b->y;
    return a->id<b->id;
}

static IntKDPoint naive_nearest(
    const IntKDPoint *points,
    size_t count,
    int64_t qx,
    int64_t qy,
    long double *out_distance
) {
    size_t best=0;
    long double distance=distance_sq(&points[0],qx,qy);

    for(size_t i=1;i<count;++i) {
        const long double candidate=
            distance_sq(&points[i],qx,qy);

        if(candidate<distance||
           (candidate==distance&&
            point_less(&points[i],&points[best]))) {
            best=i;
            distance=candidate;
        }
    }

    *out_distance=distance;
    return points[best];
}

static void test_basic(void) {
    const IntKDPoint points[]={
        {2,3,1},{5,4,2},{9,6,3},
        {4,7,4},{8,1,5},{7,2,6}
    };

    const size_t n=sizeof points/sizeof points[0];

    IntKDTree *tree=int_kd_tree_create(points,n);
    assert(tree!=NULL);
    assert(int_kd_tree_validate(tree));

    size_t count=0;

    assert(int_kd_tree_query_count(
        tree,3,9,1,6,&count
    ));
    assert(count==3);

    IntKDPoint actual;
    long double actual_distance=0.0L;
    long double expected_distance=0.0L;

    IntKDPoint expected=naive_nearest(
        points,n,6,3,&expected_distance
    );

    assert(int_kd_tree_nearest(
        tree,6,3,&actual,&actual_distance
    ));

    assert(actual.x==expected.x);
    assert(actual.y==expected.y);
    assert(actual.id==expected.id);
    assert(actual_distance==expected_distance);

    int_kd_tree_free(tree);
}

static void test_duplicates(void) {
    const IntKDPoint points[]={
        {1,1,9},{1,1,3},{1,1,7},{2,2,1}
    };

    IntKDTree *tree=int_kd_tree_create(points,4);
    assert(tree!=NULL);
    assert(int_kd_tree_validate(tree));

    IntKDPoint nearest;
    long double distance=0.0L;

    assert(int_kd_tree_nearest(
        tree,1,1,&nearest,&distance
    ));

    assert(distance==0.0L);
    assert(nearest.id==3);

    int_kd_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=1000, QUERIES=12000 };

    IntKDPoint *points=malloc(N*sizeof *points);
    IntKDPoint *output=malloc(N*sizeof *output);

    assert(points!=NULL&&output!=NULL);

    uint32_t rng=0x54A12345u;

    for(size_t i=0;i<N;++i) {
        points[i]=(IntKDPoint){
            .x=(int64_t)(next_rng(&rng)%2001U)-1000,
            .y=(int64_t)(next_rng(&rng)%2001U)-1000,
            .id=(uint64_t)i+1
        };
    }

    IntKDTree *tree=int_kd_tree_create(points,N);
    assert(tree!=NULL);
    assert(int_kd_tree_validate(tree));

    for(int q=0;q<QUERIES;++q) {
        if((q&1)==0) {
            int64_t xl=
                (int64_t)(next_rng(&rng)%2201U)-1100;
            int64_t xh=
                (int64_t)(next_rng(&rng)%2201U)-1100;
            int64_t yl=
                (int64_t)(next_rng(&rng)%2201U)-1100;
            int64_t yh=
                (int64_t)(next_rng(&rng)%2201U)-1100;

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

            assert(int_kd_tree_query_count(
                tree,xl,xh,yl,yh,&actual
            ));
            assert(actual==expected);

            if((q%101)==0) {
                size_t written=0;

                assert(int_kd_tree_query_report(
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
        } else {
            const int64_t qx=
                (int64_t)(next_rng(&rng)%2401U)-1200;
            const int64_t qy=
                (int64_t)(next_rng(&rng)%2401U)-1200;

            long double expected_distance=0.0L;

            const IntKDPoint expected=
                naive_nearest(
                    points,N,qx,qy,&expected_distance
                );

            IntKDPoint actual;
            long double actual_distance=0.0L;

            assert(int_kd_tree_nearest(
                tree,qx,qy,
                &actual,&actual_distance
            ));

            assert(actual.x==expected.x);
            assert(actual.y==expected.y);
            assert(actual.id==expected.id);
            assert(actual_distance==expected_distance);
        }
    }

    int_kd_tree_free(tree);
    free(output);
    free(points);
}

static void test_sizes(void) {
    for(size_t n=1;n<=129;++n) {
        IntKDPoint *points=malloc(n*sizeof *points);
        assert(points!=NULL);

        for(size_t i=0;i<n;++i) {
            points[i]=(IntKDPoint){
                .x=(int64_t)(i%5),
                .y=(int64_t)((i*7U)%11U),
                .id=(uint64_t)i
            };
        }

        IntKDTree *tree=int_kd_tree_create(points,n);

        assert(tree!=NULL);
        assert(int_kd_tree_validate(tree));

        int_kd_tree_free(tree);
        free(points);
    }
}

int main(void) {
    IntKDTree *empty=int_kd_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(int_kd_tree_validate(empty));

    IntKDPoint point;
    long double distance=0.0L;

    assert(!int_kd_tree_nearest(
        empty,0,0,&point,&distance
    ));

    int_kd_tree_free(empty);

    test_basic();
    test_duplicates();
    test_randomized();
    test_sizes();

    puts("KD-Tree tests passed");
    return 0;
}
