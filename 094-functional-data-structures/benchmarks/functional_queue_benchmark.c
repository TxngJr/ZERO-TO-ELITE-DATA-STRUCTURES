#include "functional_queue.h"
#include <stdio.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){const size_t n=200000;FQueueArena*a=fq_arena_create();if(!a)return 1;FunctionalQueue q=fq_empty();struct timespec x,y,z;timespec_get(&x,TIME_UTC);for(size_t i=0;i<n;++i){FunctionalQueue next;if(!fq_enqueue(a,q,(int)i,&next))return 2;q=next;}timespec_get(&y,TIME_UTC);volatile long long sum=0;for(size_t i=0;i<n;++i){int v=0;FunctionalQueue next;if(!fq_dequeue(a,q,&v,&next))return 3;sum+=v;q=next;}timespec_get(&z,TIME_UTC);printf("n=%zu enqueue=%.9f dequeue=%.9f arena_nodes=%zu checksum=%lld\n",n,e(x,y),e(y,z),fq_arena_allocated_nodes(a),sum);fq_arena_free(a);return 0;}
