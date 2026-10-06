#include "cow_int_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=1000000,c=2000;int*v=malloc(n*sizeof*v);if(!v)return 1;for(size_t i=0;i<n;++i)v[i]=(int)i;CowIntVector*base=cow_vector_from_array(v,n);if(!base)return 2;CowIntVector**copies=malloc(c*sizeof*copies);if(!copies)return 3;struct timespec a,b,d;timespec_get(&a,TIME_UTC);for(size_t i=0;i<c;++i){copies[i]=cow_vector_clone(base);if(!copies[i])return 4;}timespec_get(&b,TIME_UTC);for(size_t i=0;i<100;++i)if(!cow_vector_set(copies[i],i,(int)(-1-(int)i)))return 5;timespec_get(&d,TIME_UTC);printf("n=%zu clones=%zu clone_time=%.9f detach_100=%.9f base_refs=%zu\n",n,c,e(a,b),e(b,d),cow_vector_share_count(base));for(size_t i=0;i<c;++i)cow_vector_free(copies[i]);free(copies);cow_vector_free(base);free(v);return 0;}
