#include "u64_cuckoo_set.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=300000;U64CuckooSet *s=u64_cuckoo_set_create(1024);if(!s)return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);for(uint64_t i=0;i<n;++i)if(!u64_cuckoo_set_insert(s,i*UINT64_C(11400714819323198485)))return 1;timespec_get(&b,TIME_UTC);printf("n=%zu table_capacity=%zu seconds=%.9f\n",n,u64_cuckoo_set_table_capacity(s),elapsed(a,b));u64_cuckoo_set_free(s);return 0;}
