#include "int_bplus_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void assert_range_reference(
    const IntBPlusTree *tree,
    const bool *present,
    int min,
    int max,
    int low,
    int high
) {
    int output[2048];
    size_t written=0;

    assert(int_bplus_tree_range(
        tree,low,high,output,2048,&written
    ));

    size_t index=0;

    for(int key=min;key<=max;++key) {
        if(key>=low && key<=high && present[key-min]) {
            assert(index<written);
            assert(output[index++]==key);
        }
    }

    assert(index==written);
}

static void test_sequential(size_t t) {
    IntBPlusTree *tree=int_bplus_tree_create(t);
    assert(tree!=NULL);

    for(int i=1;i<=500;++i) {
        assert(int_bplus_tree_insert(tree,i));
        assert(int_bplus_tree_validate(tree));
    }

    int output[101];
    size_t written=0;

    assert(int_bplus_tree_range(tree,100,200,output,101,&written));
    assert(written==101);

    for(size_t i=0;i<written;++i) {
        assert(output[i]==100+(int)i);
    }

    for(int i=1;i<=500;i+=2) {
        assert(int_bplus_tree_remove(tree,i));
        assert(int_bplus_tree_validate(tree));
    }

    for(int i=1;i<=500;++i) {
        assert(int_bplus_tree_contains(tree,i)==((i%2)==0));
    }

    int_bplus_tree_free(tree);
}

static void test_randomized(size_t t) {
    enum { MIN=-500,MAX=500,COUNT=1001,STEPS=25000 };

    IntBPlusTree *tree=int_bplus_tree_create(t);
    assert(tree!=NULL);

    bool present[COUNT];
    memset(present,0,sizeof present);

    uint32_t rng=(uint32_t)(0xB5100000u+t);
    size_t expected=0;

    for(int step=0;step<STEPS;++step) {
        const int key=MIN+(int)(next_rng(&rng)%COUNT);
        const size_t idx=(size_t)(key-MIN);
        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const bool inserted=int_bplus_tree_insert(tree,key);
            assert(inserted==!present[idx]);
            if(inserted){present[idx]=true;++expected;}
        } else if(op==1U) {
            const bool removed=int_bplus_tree_remove(tree,key);
            assert(removed==present[idx]);
            if(removed){present[idx]=false;--expected;}
        } else {
            assert(int_bplus_tree_contains(tree,key)==present[idx]);
        }

        assert(int_bplus_tree_size(tree)==expected);
        assert(int_bplus_tree_validate(tree));

        if((step%401)==0) {
            const int a=MIN+(int)(next_rng(&rng)%COUNT);
            const int b=MIN+(int)(next_rng(&rng)%COUNT);
            const int low=a<b?a:b;
            const int high=a<b?b:a;

            assert_range_reference(
                tree,present,MIN,MAX,low,high
            );
        }
    }

    int_bplus_tree_free(tree);
}

int main(void) {
    assert(int_bplus_tree_create(1)==NULL);

    const size_t ts[]={2,3,8};

    for(size_t i=0;i<3;++i) {
        test_sequential(ts[i]);
        test_randomized(ts[i]);
    }

    puts("B+ Tree tests passed");
    return 0;
}
