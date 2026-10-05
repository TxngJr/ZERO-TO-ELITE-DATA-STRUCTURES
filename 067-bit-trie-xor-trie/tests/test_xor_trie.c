#include "int_xor_trie.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t next_rng(uint64_t *state) {
    *state=*state*UINT64_C(6364136223846793005)+
        UINT64_C(1442695040888963407);
    return *state;
}

static bool model_contains(
    const uint64_t *values,
    size_t n,
    uint64_t value
) {
    for(size_t i=0;i<n;++i) {
        if(values[i]==value)return true;
    }
    return false;
}

static void model_remove(
    uint64_t *values,
    size_t *n,
    uint64_t value
) {
    for(size_t i=0;i<*n;++i) {
        if(values[i]==value) {
            values[i]=values[*n-1];
            --*n;
            return;
        }
    }

    assert(false);
}

static uint64_t naive_extreme_score(
    const uint64_t *values,
    size_t n,
    uint64_t query,
    bool maximize
) {
    assert(n>0);

    uint64_t best=values[0]^query;

    for(size_t i=1;i<n;++i) {
        const uint64_t score=values[i]^query;

        if((maximize&&score>best)||
           (!maximize&&score<best)) {
            best=score;
        }
    }

    return best;
}

static size_t naive_count_less(
    const uint64_t *values,
    size_t n,
    uint64_t query,
    uint64_t limit
) {
    size_t count=0;

    for(size_t i=0;i<n;++i) {
        if((values[i]^query)<limit)++count;
    }

    return count;
}

static void test_known(void) {
    IntXorTrie *trie=int_xor_trie_create();
    assert(trie!=NULL);

    assert(int_xor_trie_insert(trie,3));
    assert(int_xor_trie_insert(trie,5));
    assert(int_xor_trie_insert(trie,14));
    assert(int_xor_trie_insert(trie,5));

    assert(int_xor_trie_validate(trie));
    assert(int_xor_trie_size(trie)==4);

    uint64_t value=0,score=0;

    assert(int_xor_trie_max_xor(
        trie,6,&value,&score
    ));
    assert(score==(14U^6U));

    assert(int_xor_trie_min_xor(
        trie,6,&value,&score
    ));
    assert(score==(5U^6U));

    size_t count=0;

    assert(int_xor_trie_count_xor_less_than(
        trie,6,8,&count
    ));
    assert(count==3);

    assert(int_xor_trie_remove(trie,5));
    assert(int_xor_trie_size(trie)==3);

    bool present=false;
    assert(int_xor_trie_contains(trie,5,&present));
    assert(present);

    assert(int_xor_trie_remove(trie,5));
    assert(int_xor_trie_contains(trie,5,&present));
    assert(!present);

    assert(!int_xor_trie_remove(trie,5));
    assert(int_xor_trie_validate(trie));

    int_xor_trie_free(trie);
}

static void test_randomized(void) {
    enum { CAP=8000, OPS=50000 };

    uint64_t *model=malloc(CAP*sizeof *model);
    assert(model!=NULL);

    size_t n=0;
    IntXorTrie *trie=int_xor_trie_create();
    assert(trie!=NULL);

    uint64_t rng=UINT64_C(0x67A12345ABCDEF01);

    for(int op=0;op<OPS;++op) {
        const unsigned kind=(unsigned)(next_rng(&rng)%6U);
        const uint64_t value=next_rng(&rng)%4096U;

        if(kind==0&&n<CAP) {
            assert(int_xor_trie_insert(trie,value));
            model[n++]=value;
        } else if(kind==1) {
            const bool exists=model_contains(model,n,value);
            const bool removed=int_xor_trie_remove(trie,value);

            assert(removed==exists);

            if(exists)model_remove(model,&n,value);
        } else if(kind==2) {
            bool actual=false;
            assert(int_xor_trie_contains(
                trie,value,&actual
            ));
            assert(actual==model_contains(model,n,value));
        } else if(kind==3&&n>0) {
            uint64_t actual_value=0,actual_score=0;

            assert(int_xor_trie_max_xor(
                trie,value,&actual_value,&actual_score
            ));

            assert(actual_score==
                   naive_extreme_score(model,n,value,true));
            assert((actual_value^value)==actual_score);
            assert(model_contains(model,n,actual_value));
        } else if(kind==4&&n>0) {
            uint64_t actual_value=0,actual_score=0;

            assert(int_xor_trie_min_xor(
                trie,value,&actual_value,&actual_score
            ));

            assert(actual_score==
                   naive_extreme_score(model,n,value,false));
            assert((actual_value^value)==actual_score);
            assert(model_contains(model,n,actual_value));
        } else {
            const uint64_t limit=next_rng(&rng)%8192U;
            const size_t expected=naive_count_less(
                model,n,value,limit
            );

            size_t actual=0;

            assert(int_xor_trie_count_xor_less_than(
                trie,value,limit,&actual
            ));

            assert(actual==expected);
        }

        assert(int_xor_trie_size(trie)==n);

        if((op%997)==0) {
            assert(int_xor_trie_validate(trie));
        }
    }

    assert(int_xor_trie_validate(trie));

    int_xor_trie_free(trie);
    free(model);
}

int main(void) {
    IntXorTrie *empty=int_xor_trie_create();
    assert(empty!=NULL);
    assert(int_xor_trie_validate(empty));

    uint64_t a=0,b=0;
    assert(!int_xor_trie_max_xor(empty,0,&a,&b));

    int_xor_trie_free(empty);

    test_known();
    test_randomized();

    puts("XOR Trie tests passed");
    return 0;
}
