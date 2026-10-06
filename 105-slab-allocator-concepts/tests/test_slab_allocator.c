#include "slab_allocator.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

enum { SLOTS=64, ACTIVE=4096, STEPS=80000 };

static uint64_t state = UINT64_C(0x123456789abcdef);
static uint64_t rng(void) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

typedef struct {
    void *ptr;
    size_t requested;
    size_t class_size;
} Record;

int main(void) {
    SlabAllocator *allocator = slab_create(SLOTS);
    assert(allocator && slab_validate(allocator));

    Record records[ACTIVE] = {0};
    size_t live=0U, requested_sum=0U, class_sum=0U;

    for (size_t step=0; step<STEPS; ++step) {
        bool do_alloc = live==0U ||
            (live<ACTIVE && (rng()%100U)<60U);

        if (do_alloc) {
            size_t pos=0U;
            while (pos<ACTIVE && records[pos].ptr) ++pos;
            assert(pos<ACTIVE);

            size_t requested=(size_t)(rng()%256U)+1U;
            size_t class_size=0U;
            void *ptr=slab_alloc(allocator,requested,&class_size);
            assert(ptr);
            assert(class_size>=requested);
            assert(class_size==16U || class_size==32U ||
                   class_size==64U || class_size==128U ||
                   class_size==256U);

            records[pos]=(Record){ptr,requested,class_size};
            ++live;
            requested_sum+=requested;
            class_sum+=class_size;
        } else {
            size_t kth=(size_t)(rng()%live),pos=0U;
            while (pos<ACTIVE) {
                if (records[pos].ptr) {
                    if (kth==0U) break;
                    --kth;
                }
                ++pos;
            }
            assert(pos<ACTIVE);

            void *ptr=records[pos].ptr;
            size_t requested=records[pos].requested;
            size_t class_size=records[pos].class_size;
            assert(slab_release(allocator,ptr));
            assert(!slab_release(allocator,ptr));

            records[pos]=(Record){0};
            --live;
            requested_sum-=requested;
            class_sum-=class_size;
        }

        assert(slab_live_objects(allocator)==live);
        assert(slab_requested_live_bytes(allocator)==requested_sum);
        assert(slab_allocated_class_bytes(allocator)==class_sum);
        if (step%997U==0U) assert(slab_validate(allocator));
    }

    for (size_t i=0;i<ACTIVE;++i)
        if (records[i].ptr)
            assert(slab_release(allocator,records[i].ptr));

    int foreign=0;
    assert(!slab_release(allocator,&foreign));
    assert(slab_live_objects(allocator)==0U);
    assert(slab_validate(allocator));

    size_t before=slab_slab_count(allocator);
    size_t trimmed=slab_trim_empty(allocator);
    assert(trimmed==before);
    assert(slab_slab_count(allocator)==0U);
    assert(slab_reserved_payload_bytes(allocator)==0U);
    assert(slab_validate(allocator));

    printf("Slab tests passed; trimmed=%zu\n",trimmed);
    slab_free_allocator(allocator);
    return 0;
}
