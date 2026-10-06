#include "cache_oblivious_matrix.h"
#include <stdio.h>
#include <time.h>

enum{N=1024,REPEATS=20};
static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    CacheObliviousMatrix*m=com_create(N,N);
    if(!m)return 1;
    for(size_t r=0;r<N;++r)for(size_t c=0;c<N;++c)
        if(!com_set(m,r,c,(double)((r+c)%97U)))return 2;

    volatile double a_sum=0.0,b_sum=0.0;
    struct timespec a,b,c;
    timespec_get(&a,TIME_UTC);
    for(int i=0;i<REPEATS;++i)a_sum+=com_sum_row_order(m);
    timespec_get(&b,TIME_UTC);
    for(int i=0;i<REPEATS;++i)b_sum+=com_sum_z_order(m);
    timespec_get(&c,TIME_UTC);

    printf("matrix=%dx%d repeats=%d row_seconds=%.6f z_seconds=%.6f checksum=%.0f/%.0f\n",
           N,N,REPEATS,elapsed(a,b),elapsed(b,c),(double)a_sum,(double)b_sum);
    com_free(m);return 0;
}
