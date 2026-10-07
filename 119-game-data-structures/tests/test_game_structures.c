#include "game_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
enum{N=50000};
int main(void){
    GameWorld*w=gw_create(60000);assert(w);EntityId*ids=malloc(N*sizeof(*ids));assert(ids);
    for(size_t i=0;i<N;++i){assert(gw_spawn(w,&ids[i]));float x=(float)(i%500U)*2.0f+0.25f;float y=(float)(i/500U)*2.0f+0.25f;assert(gw_set_position(w,ids[i],x,y));}
    assert(gw_entity_count(w)==N&&gw_position_count(w)==N&&gw_validate(w));
    EntityId stale=ids[0];assert(gw_destroy(w,stale));EntityId replacement=0;assert(gw_spawn(w,&replacement));assert(replacement!=stale);assert(gw_set_position(w,replacement,0.25f,0.25f));float x,y;assert(!gw_get_position(w,stale,&x,&y));
    SpatialGrid*g=grid_create(gw_capacity(w),0,0,1000,200,10);assert(g&&grid_rebuild(g,w)&&grid_validate(g,w));
    EntityId*out=malloc(2000*sizeof(*out));assert(out);size_t count=0;
    assert(grid_query_aabb(g,w,100.0f,50.0f,199.9f,99.9f,out,2000,&count));assert(count==1250U);
    for(size_t i=10;i<N;i+=10)if(ids[i]!=stale)assert(gw_destroy(w,ids[i]));
    assert(!grid_query_aabb(g,w,0,0,10,10,out,2000,&count));
    assert(gw_validate(w)&&grid_rebuild(g,w)&&grid_validate(g,w));
    bool removed=false;assert(gw_remove_position(w,replacement,&removed)&&removed);assert(!grid_validate(g,w));
    assert(grid_rebuild(g,w)&&grid_validate(g,w));
    printf("Game structures tests passed; entities=%zu positions=%zu version=%llu\n",gw_entity_count(w),gw_position_count(w),(unsigned long long)gw_version(w));
    free(out);grid_free(g);free(ids);gw_free(w);return 0;
}
