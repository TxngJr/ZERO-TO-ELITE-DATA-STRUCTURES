#include "game_structures.h"
#include <stdio.h>
int main(void){GameWorld*w=gw_create(16);if(!w)return 1;EntityId e;if(!gw_spawn(w,&e)||!gw_set_position(w,e,3.0f,4.0f))return 2;SpatialGrid*g=grid_create(16,0,0,100,100,10);if(!g||!grid_rebuild(g,w))return 3;EntityId out[4];size_t n=0;if(!grid_query_aabb(g,w,0,0,10,10,out,4,&n))return 4;printf("matches=%zu entity=%llu\n",n,(unsigned long long)e);grid_free(g);gw_free(w);return 0;}
