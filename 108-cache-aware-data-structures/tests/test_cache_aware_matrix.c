#include "cache_aware_matrix.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{ROWS=257,COLS=193,UPDATES=100000};
static uint64_t state=UINT64_C(0x314159265358979);
static uint64_t rng(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    size_t n=(size_t)ROWS*COLS;
    double*dense=calloc(n,sizeof(*dense));
    double*copy=malloc(n*sizeof(*copy));
    double*trans=malloc(n*sizeof(*trans));
    assert(dense&&copy&&trans);

    CacheAwareMatrix*m=cam_create(ROWS,COLS,16U,16U);
    assert(m&&cam_validate(m));

    for(size_t i=0;i<UPDATES;++i){
        size_t r=(size_t)(rng()%ROWS),c=(size_t)(rng()%COLS);
        double value=(double)(int)(rng()%10001U);
        dense[r*COLS+c]=value;
        assert(cam_set(m,r,c,value));
        if(i%997U==0U){
            double got=0.0;
            assert(cam_get(m,r,c,&got)&&got==value);
        }
    }

    assert(cam_copy_to_dense(m,copy,n));
    for(size_t i=0;i<n;++i)assert(copy[i]==dense[i]);
    assert(cam_sum_row_order(m)==cam_sum_tile_order(m));

    assert(cam_transpose_to_dense(m,trans,n));
    for(size_t r=0;r<ROWS;++r)
        for(size_t c=0;c<COLS;++c)
            assert(trans[c*ROWS+r]==dense[r*COLS+c]);

    assert(cam_validate(m));
    printf("Cache-aware matrix tests passed; %zux%zu tile=%zux%zu\n",
           cam_rows(m),cam_cols(m),cam_tile_rows(m),cam_tile_cols(m));

    cam_free(m);
    free(dense);free(copy);free(trans);
    return 0;
}
