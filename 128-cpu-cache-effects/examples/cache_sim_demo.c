#include "cache_sim.h"
#include <stdio.h>
int main(void){CacheSim*s=cache_sim_create(64,4,2);if(!s)return 1;uint64_t a[]={0,8,64,0,256,0};if(!cache_sim_access_range(s,a,6))return 2;CacheStats st=cache_sim_stats(s);printf("accesses=%llu hits=%llu misses=%llu evictions=%llu\n",(unsigned long long)st.accesses,(unsigned long long)st.hits,(unsigned long long)st.misses,(unsigned long long)st.evictions);cache_sim_free(s);return 0;}
