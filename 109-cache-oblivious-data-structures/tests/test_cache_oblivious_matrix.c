#include "cache_oblivious_matrix.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{ROWS=257,COLS=193,UPDATES=100000};
static uint64_t state=UINT64_C(0xabcddcba13579);
static uint64_t rng(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    size_t n=(size_t)ROWS*COLS;
    double*dense=calloc(n,sizeof(*dense));
    double*copy=malloc(n*sizeof(*copy));
    double*trans=malloc(n*sizeof(*trans));
    assert(dense&&copy&&trans);

    CacheObliviousMatrix*m=com_create(ROWS,COLS);
    assert(m&&com_padded_side(m)==512U&&com_validate(m));

    for(size_t i=0;i<UPDATES;++i){
        size_t r=(size_t)(rng()%ROWS),c=(size_t)(rng()%COLS);
        double value=(double)(rng()%100000U);
        dense[r*COLS+c]=value;
        assert(com_set(m,r,c,value));
    }

    assert(com_copy_to_dense(m,copy,n));
    for(size_t i=0;i<n;++i)assert(copy[i]==dense[i]);
    assert(com_sum_row_order(m)==com_sum_z_order(m));

    assert(com_transpose_to_dense(m,trans,n));
    for(size_t r=0;r<ROWS;++r)
        for(size_t c=0;c<COLS;++c)
            assert(trans[c*ROWS+r]==dense[r*COLS+c]);

    assert(com_validate(m));
    printf("Cache-oblivious matrix tests passed; side=%zu\n",com_padded_side(m));
    com_free(m);free(dense);free(copy);free(trans);
    return 0;
}
