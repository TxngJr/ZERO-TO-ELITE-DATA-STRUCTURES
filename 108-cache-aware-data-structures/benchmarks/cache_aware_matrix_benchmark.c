#include "cache_aware_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum{N=1024,REPEATS=20};

static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    size_t count=(size_t)N*N;
    double*dense=malloc(count*sizeof(*dense));
    double*trans=malloc(count*sizeof(*trans));
    CacheAwareMatrix*m=cam_create(N,N,32U,32U);
    if(!dense||!trans||!m)return 1;

    for(size_t i=0;i<count;++i)dense[i]=(double)(i%97U);
    if(!cam_fill_from_dense(m,dense,count))return 2;

    struct timespec a,b,c,d;
    volatile double row_sum=0.0,tile_sum=0.0;
    timespec_get(&a,TIME_UTC);
    for(int i=0;i<REPEATS;++i)row_sum+=cam_sum_row_order(m);
    timespec_get(&b,TIME_UTC);
    for(int i=0;i<REPEATS;++i)tile_sum+=cam_sum_tile_order(m);
    timespec_get(&c,TIME_UTC);
    if(!cam_transpose_to_dense(m,trans,count))return 3;
    timespec_get(&d,TIME_UTC);

    printf("matrix=%dx%d repeats=%d row_seconds=%.6f "
           "tile_seconds=%.6f transpose_seconds=%.6f "
           "checksum=%.0f/%.0f\n",
           N,N,REPEATS,elapsed(a,b),elapsed(b,c),elapsed(c,d),
           (double)row_sum,(double)tile_sum);

    cam_free(m);
    free(dense);free(trans);
    return 0;
}
