#include "int_order_stat_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    DOMAIN_LOW=-2000,
    DOMAIN_HIGH=2000,
    DOMAIN_SIZE=4001
};

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t ref_size(const bool *present) {
    size_t count=0;

    for(size_t i=0;i<DOMAIN_SIZE;++i) {
        if(present[i])++count;
    }

    return count;
}

static size_t ref_rank(
    const bool *present,
    int64_t key
) {
    size_t rank=0;

    for(int value=DOMAIN_LOW;
        value<=DOMAIN_HIGH;
        ++value) {
        if((int64_t)value>=key)break;

        if(present[(size_t)(value-DOMAIN_LOW)]) {
            ++rank;
        }
    }

    return rank;
}

static bool ref_select(
    const bool *present,
    size_t rank,
    int64_t *out
) {
    size_t seen=0;

    for(int value=DOMAIN_LOW;
        value<=DOMAIN_HIGH;
        ++value) {
        if(!present[(size_t)(value-DOMAIN_LOW)]) {
            continue;
        }

        if(seen==rank) {
            *out=value;
            return true;
        }

        ++seen;
    }

    return false;
}

static void test_basic(void) {
    IntOrderStatTree *tree=int_order_stat_tree_create();
    assert(tree!=NULL);

    const int64_t values[]={40,20,60,10,30,50,70};

    for(size_t i=0;i<7;++i) {
        assert(int_order_stat_tree_insert(tree,values[i]));
    }

    int64_t key=0;
    size_t rank=0;

    assert(int_order_stat_tree_select(tree,0,&key));
    assert(key==10);

    assert(int_order_stat_tree_select(tree,6,&key));
    assert(key==70);

    assert(int_order_stat_tree_rank(tree,40,&rank));
    assert(rank==3);

    assert(int_order_stat_tree_rank(tree,55,&rank));
    assert(rank==5);

    assert(int_order_stat_tree_count_range(
        tree,25,65,&rank
    ));
    assert(rank==4);

    assert(int_order_stat_tree_remove(tree,40));
    assert(int_order_stat_tree_validate(tree));

    int_order_stat_tree_free(tree);
}

static void test_randomized(void) {
    enum { STEPS=40000 };

    bool *present=calloc(DOMAIN_SIZE,sizeof *present);
    assert(present!=NULL);

    IntOrderStatTree *tree=int_order_stat_tree_create();
    assert(tree!=NULL);

    uint32_t rng=0x52A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%5U;
        const int value=
            DOMAIN_LOW+
            (int)(next_rng(&rng)%DOMAIN_SIZE);
        const size_t slot=(size_t)(value-DOMAIN_LOW);

        if(op==0U) {
            const bool expected=!present[slot];
            const bool actual=int_order_stat_tree_insert(
                tree,value
            );

            assert(actual==expected);
            if(actual)present[slot]=true;
        } else if(op==1U) {
            const bool expected=present[slot];
            const bool actual=int_order_stat_tree_remove(
                tree,value
            );

            assert(actual==expected);
            if(actual)present[slot]=false;
        } else if(op==2U) {
            size_t actual=0;
            assert(int_order_stat_tree_rank(
                tree,value,&actual
            ));
            assert(actual==ref_rank(present,value));
        } else if(op==3U) {
            const size_t size=ref_size(present);

            if(size==0) {
                int64_t dummy=0;
                assert(!int_order_stat_tree_select(
                    tree,0,&dummy
                ));
            } else {
                const size_t rank=next_rng(&rng)%size;
                int64_t expected=0;
                int64_t actual=0;

                assert(ref_select(
                    present,rank,&expected
                ));
                assert(int_order_stat_tree_select(
                    tree,rank,&actual
                ));
                assert(actual==expected);
            }
        } else {
            int a=DOMAIN_LOW+
                (int)(next_rng(&rng)%DOMAIN_SIZE);
            int b=DOMAIN_LOW+
                (int)(next_rng(&rng)%DOMAIN_SIZE);

            if(a>b) {
                const int tmp=a;a=b;b=tmp;
            }

            if(a==b) {
                if(b<DOMAIN_HIGH)++b;
                else --a;
            }

            size_t actual=0;
            assert(int_order_stat_tree_count_range(
                tree,a,b,&actual
            ));

            const size_t expected=
                ref_rank(present,b)-
                ref_rank(present,a);

            assert(actual==expected);
        }

        if((step%257)==0) {
            assert(int_order_stat_tree_size(tree)==
                   ref_size(present));
            assert(int_order_stat_tree_validate(tree));
        }
    }

    assert(int_order_stat_tree_validate(tree));

    free(present);
    int_order_stat_tree_free(tree);
}

int main(void) {
    test_basic();
    test_randomized();
    puts("Order Statistic Tree tests passed");
    return 0;
}
