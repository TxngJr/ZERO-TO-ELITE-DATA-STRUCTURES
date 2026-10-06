#include "free_list_allocator.h"
#include <stdio.h>

int main(void){
    FreeListAllocator*a=fla_create(1024U);
    if(!a)return 1;
    void*x=fla_alloc(a,100U,16U);
    void*y=fla_alloc(a,200U,32U);
    if(!x||!y)return 2;
    printf("allocated=%zu free=%zu blocks=%zu largest=%zu\n",
           fla_allocated_bytes(a),fla_free_bytes(a),
           fla_free_block_count(a),fla_largest_free_block(a));
    if(!fla_release(a,x)||!fla_release(a,y))return 3;
    printf("after release free-blocks=%zu largest=%zu\n",
           fla_free_block_count(a),fla_largest_free_block(a));
    fla_free(a);
    return 0;
}
