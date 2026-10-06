#include "persistent_int_set.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static double elapsed(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){
    const size_t n=20000;PSetArena*a=pset_arena_create();if(!a)return 1;const PSetNode*root=NULL;
    struct timespec x,y;timespec_get(&x,TIME_UTC);
    for(size_t i=0;i<n;++i){uint32_t key=(uint32_t)(i*UINT32_C(2654435761))&UINT32_C(0x7fffffff);const PSetNode*next=NULL;bool changed=false;if(!pset_insert(a,root,(int)key,&next,&changed)||!changed)return 2;root=next;}
    timespec_get(&y,TIME_UTC);
    printf("versions=%zu final_size=%zu height=%zu arena_nodes=%zu blocks=%zu build=%.9f\n",n,pset_size(root),pset_height(root),pset_arena_allocated_nodes(a),pset_arena_allocated_blocks(a),elapsed(x,y));
    if(!pset_validate(root)) return 3;
    pset_arena_free(a);
    return 0;
}
