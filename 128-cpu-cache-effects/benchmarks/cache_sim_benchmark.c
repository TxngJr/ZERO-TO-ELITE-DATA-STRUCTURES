#include "cache_sim.h"
#include <stdio.h>
#include <time.h>
enum{N=5000000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
static void run(size_t ways){CacheSim*s=cache_sim_create(64,1024,ways);if(!s)return;struct timespec a,b;timespec_get(&a,TIME_UTC);for(size_t i=0;i<N;++i){uint64_t line=(uint64_t)((i*8191U)%131072U);cache_sim_access(s,line*64U,NULL);}timespec_get(&b,TIME_UTC);CacheStats st=cache_sim_stats(s);printf("ways=%zu seconds=%.6f hits=%llu misses=%llu evictions=%llu\n",ways,e(a,b),(unsigned long long)st.hits,(unsigned long long)st.misses,(unsigned long long)st.evictions);cache_sim_free(s);}
int main(void){run(1);run(2);run(4);run(8);return 0;}
