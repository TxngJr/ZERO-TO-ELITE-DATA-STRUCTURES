#include "game_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum{N=100000,Q=10000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){GameWorld*w=gw_create(N);if(!w)return 1;EntityId id;for(size_t i=0;i<N;++i){if(!gw_spawn(w,&id)||!gw_set_position(w,id,(float)(i%1000U)+.1f,(float)(i/1000U)+.1f))return 2;}SpatialGrid*g=grid_create(N,0,0,1000,100,10);if(!g)return 3;EntityId*out=malloc(1000*sizeof(*out));if(!out)return 4;struct timespec a,b,c;timespec_get(&a,TIME_UTC);if(!grid_rebuild(g,w))return 5;timespec_get(&b,TIME_UTC);size_t sum=0;for(size_t q=0;q<Q;++q){float x=(float)(q%990U);size_t n=0;if(!grid_query_aabb(g,w,x,10,x+9.9f,19.9f,out,1000,&n))return 6;sum+=n;}timespec_get(&c,TIME_UTC);printf("entities=%d rebuild_seconds=%.6f query_seconds=%.6f checksum=%zu\n",N,e(a,b),e(b,c),sum);free(out);grid_free(g);gw_free(w);return 0;}
