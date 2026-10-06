#include "slab_allocator.h"
#include <stdio.h>
#include <time.h>

enum { TARGET=500000, ACTIVE=2048 };

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void) {
    SlabAllocator *allocator = slab_create(128U);
    if (!allocator) return 1;

    void *ptrs[ACTIVE];
    size_t classes[ACTIVE];
    struct timespec begin,end;
    timespec_get(&begin,TIME_UTC);

    int rounds=TARGET/ACTIVE;
    for (int round=0;round<rounds;++round) {
        for (int i=0;i<ACTIVE;++i) {
            size_t request=(size_t)((i*37)%256)+1U;
            ptrs[i]=slab_alloc(allocator,request,&classes[i]);
            if (!ptrs[i]) return 2;
        }
        for (int i=0;i<ACTIVE;++i)
            if (!slab_release(allocator,ptrs[i])) return 3;
    }

    timespec_get(&end,TIME_UTC);
    printf("operations=%d seconds=%.6f slabs=%zu reserved=%zu\n",
           rounds*ACTIVE*2,
           elapsed(begin,end),
           slab_slab_count(allocator),
           slab_reserved_payload_bytes(allocator));

    slab_free_allocator(allocator);
    return 0;
}
