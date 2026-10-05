#include "int_btree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void assert_reference(
    const IntBTree *tree,
    const bool *present,
    int min,
    int max
) {
    int output[2048];
    size_t written=0;

    assert(int_btree_inorder(tree,output,2048,&written));

    size_t index=0;
    for(int key=min;key<=max;++key) {
        if(present[key-min]) {
            assert(index<written);
            assert(output[index++]==key);
        }
    }
    assert(index==written);
}

static void test_sequential(size_t t) {
    IntBTree *tree=int_btree_create(t);
    assert(tree!=NULL);

    for(int i=1;i<=500;++i) {
        assert(int_btree_insert(tree,i));
        assert(int_btree_validate(tree));
    }

    assert(!int_btree_insert(tree,250));

    for(int i=1;i<=500;i+=3) {
        assert(int_btree_remove(tree,i));
        assert(int_btree_validate(tree));
    }

    int_btree_free(tree);
}

static void test_randomized(size_t t) {
    enum { MIN=-500,MAX=500,COUNT=1001,STEPS=30000 };

    IntBTree *tree=int_btree_create(t);
    assert(tree!=NULL);

    bool present[COUNT];
    memset(present,0,sizeof present);

    uint32_t rng=(uint32_t)(0xB7000000u+t);
    size_t expected=0;

    for(int step=0;step<STEPS;++step) {
        const int key=MIN+(int)(next_rng(&rng)%COUNT);
        const size_t idx=(size_t)(key-MIN);
        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const bool inserted=int_btree_insert(tree,key);
            assert(inserted==!present[idx]);
            if(inserted){present[idx]=true;++expected;}
        } else if(op==1U) {
            const bool removed=int_btree_remove(tree,key);
            assert(removed==present[idx]);
            if(removed){present[idx]=false;--expected;}
        } else {
            assert(int_btree_contains(tree,key)==present[idx]);
        }

        assert(int_btree_size(tree)==expected);
        assert(int_btree_validate(tree));

        if((step%313)==0) {
            assert_reference(tree,present,MIN,MAX);
        }
    }

    assert_reference(tree,present,MIN,MAX);
    int_btree_free(tree);
}

int main(void) {
    assert(int_btree_create(1)==NULL);

    const size_t ts[]={2,3,8};

    for(size_t i=0;i<3;++i) {
        test_sequential(ts[i]);
        test_randomized(ts[i]);
    }

    puts("B-Tree tests passed");
    return 0;
}
