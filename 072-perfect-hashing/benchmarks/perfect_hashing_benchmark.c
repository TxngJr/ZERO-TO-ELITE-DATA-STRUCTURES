#include "u64_perfect_set.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=100000;uint64_t *keys=malloc(n*sizeof *keys);if(!keys)return 1;for(size_t i=0;i<n;++i)keys[i]=(uint64_t)i*UINT64_C(0x9e3779b97f4a7c15);struct timespec a,b;timespec_get(&a,TIME_UTC);U64PerfectSet *s=u64_perfect_set_create(keys,n);timespec_get(&b,TIME_UTC);if(!s)return 1;volatile size_t hits=0;for(size_t i=0;i<n;++i)hits+=u64_perfect_set_contains(s,keys[i]);printf("n=%zu secondary=%zu build=%.9f hits=%zu\n",n,u64_perfect_set_secondary_slots(s),elapsed(a,b),(size_t)hits);u64_perfect_set_free(s);free(keys);return 0;}
