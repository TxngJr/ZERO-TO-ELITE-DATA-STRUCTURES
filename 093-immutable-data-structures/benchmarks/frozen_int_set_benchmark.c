#include "frozen_int_set.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){
    const size_t n=200000,q=1000000;int *v=malloc(n*sizeof*v);if(!v)return 1;
    for(size_t i=0;i<n;++i)v[i]=(int)((uint32_t)i*UINT32_C(2654435761));
    struct timespec a,b,c,d;timespec_get(&a,TIME_UTC);FrozenIntSet*s=frozen_int_set_create(v,n);timespec_get(&b,TIME_UTC);if(!s)return 2;
    volatile size_t hits=0;timespec_get(&c,TIME_UTC);for(size_t i=0;i<q;++i){int key=(int)((uint32_t)(i%(n*2U))*UINT32_C(2654435761));hits+=frozen_int_set_contains(s,key);}timespec_get(&d,TIME_UTC);
    printf("input=%zu unique=%zu capacity=%zu load=%.3f bytes=%zu build=%.9f queries=%zu query_time=%.9f hits=%zu\n",n,frozen_int_set_size(s),frozen_int_set_capacity(s),frozen_int_set_load_factor(s),frozen_int_set_storage_bytes(s),elapsed(a,b),q,elapsed(c,d),(size_t)hits);
    frozen_int_set_free(s);free(v);return 0;
}
