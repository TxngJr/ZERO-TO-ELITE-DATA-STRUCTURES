#include "int_interval_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int64_t low;
    int64_t high;
    bool present;
} RefInterval;

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool overlap(
    int64_t a_low,int64_t a_high,
    int64_t b_low,int64_t b_high
) {
    return a_low<b_high&&b_low<a_high;
}

static bool ref_contains(
    const RefInterval *items,
    size_t count,
    int64_t low,
    int64_t high
) {
    for(size_t i=0;i<count;++i) {
        if(items[i].present&&
           items[i].low==low&&
           items[i].high==high) {
            return true;
        }
    }
    return false;
}

static size_t ref_count_overlap(
    const RefInterval *items,
    size_t count,
    int64_t low,
    int64_t high
) {
    size_t result=0;

    for(size_t i=0;i<count;++i) {
        if(items[i].present&&
           overlap(
               items[i].low,items[i].high,
               low,high
           )) {
            ++result;
        }
    }

    return result;
}

static void test_basic(void) {
    IntIntervalTree *tree=int_interval_tree_create();
    assert(tree!=NULL);

    assert(int_interval_tree_insert(tree,15,20));
    assert(int_interval_tree_insert(tree,10,30));
    assert(int_interval_tree_insert(tree,17,19));
    assert(int_interval_tree_insert(tree,5,20));
    assert(int_interval_tree_insert(tree,12,15));
    assert(int_interval_tree_insert(tree,30,40));

    assert(!int_interval_tree_insert(tree,15,20));
    assert(!int_interval_tree_insert(tree,4,4));

    IntInterval hit;
    assert(int_interval_tree_find_overlap(
        tree,14,16,&hit
    ));
    assert(overlap(hit.low,hit.high,14,16));

    assert(!int_interval_tree_find_overlap(
        tree,40,45,&hit
    ));

    size_t count=0;
    assert(int_interval_tree_count_overlaps(
        tree,14,16,&count
    ));
    assert(count==4);

    /* Touching endpoints are not overlap. */
    assert(int_interval_tree_insert(tree,40,50));
    assert(!int_interval_tree_find_overlap(
        tree,50,60,&hit
    ));

    assert(int_interval_tree_remove(tree,10,30));
    assert(!int_interval_tree_contains(tree,10,30));
    assert(int_interval_tree_validate(tree));

    int_interval_tree_free(tree);
}

static void test_randomized(void) {
    enum { CAPACITY=4096, STEPS=30000 };

    IntIntervalTree *tree=int_interval_tree_create();
    assert(tree!=NULL);

    RefInterval ref[CAPACITY];
    memset(ref,0,sizeof ref);

    size_t used=0;
    uint32_t rng=0x49A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%4U;

        int64_t low=(int64_t)(next_rng(&rng)%2000U)-1000;
        int64_t length=(int64_t)(next_rng(&rng)%50U)+1;
        int64_t high=low+length;

        if(op==0U) {
            const bool existed=ref_contains(
                ref,used,low,high
            );

            const bool actual=int_interval_tree_insert(
                tree,low,high
            );

            assert(actual==!existed);

            if(actual) {
                size_t slot=CAPACITY;

                for(size_t i=0;i<used;++i) {
                    if(!ref[i].present) {
                        slot=i;
                        break;
                    }
                }

                if(slot==CAPACITY) {
                    assert(used<CAPACITY);
                    slot=used++;
                }

                ref[slot]=(RefInterval){
                    .low=low,
                    .high=high,
                    .present=true
                };
            }
        } else if(op==1U) {
            bool existed=false;
            size_t slot=0;

            for(size_t i=0;i<used;++i) {
                if(ref[i].present&&
                   ref[i].low==low&&
                   ref[i].high==high) {
                    existed=true;
                    slot=i;
                    break;
                }
            }

            const bool actual=int_interval_tree_remove(
                tree,low,high
            );

            assert(actual==existed);
            if(actual)ref[slot].present=false;
        } else {
            const size_t expected=ref_count_overlap(
                ref,used,low,high
            );

            size_t actual_count=0;
            assert(int_interval_tree_count_overlaps(
                tree,low,high,&actual_count
            ));
            assert(actual_count==expected);

            IntInterval hit;
            const bool found=int_interval_tree_find_overlap(
                tree,low,high,&hit
            );

            assert(found==(expected>0));

            if(found) {
                assert(overlap(
                    hit.low,hit.high,low,high
                ));
                assert(ref_contains(
                    ref,used,hit.low,hit.high
                ));
            }
        }

        if((step%193)==0) {
            assert(int_interval_tree_validate(tree));
        }
    }

    assert(int_interval_tree_validate(tree));
    int_interval_tree_free(tree);
}

int main(void) {
    test_basic();
    test_randomized();
    puts("Interval Tree tests passed");
    return 0;
}
