#include "free_list_allocator.h"
#include <stdio.h>
#include <time.h>

enum{ACTIVE=1024,ROUNDS=1000};

static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    FreeListAllocator*a=fla_create(8U*1024U*1024U);
    if(!a)return 1;
    void*ptrs[ACTIVE];
    struct timespec begin,end;
    timespec_get(&begin,TIME_UTC);

    for(int round=0;round<ROUNDS;++round){
        for(int i=0;i<ACTIVE;++i){
            size_t size=(size_t)((i*37)%512)+1U;
            ptrs[i]=fla_alloc(a,size,16U);
            if(!ptrs[i])return 2;
        }
        for(int i=0;i<ACTIVE;++i)
            if(!fla_release(a,ptrs[i]))return 3;
    }

    timespec_get(&end,TIME_UTC);
    printf("operations=%d seconds=%.6f final_free_blocks=%zu\n",
           ROUNDS*ACTIVE*2,elapsed(begin,end),fla_free_block_count(a));
    fla_free(a);
    return 0;
}
