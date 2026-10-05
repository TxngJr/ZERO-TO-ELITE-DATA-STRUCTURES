#include "int_bitset.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t model_count(const uint8_t *m,size_t n) {
    size_t c=0;
    for(size_t i=0;i<n;++i)c+=m[i]!=0;
    return c;
}

static void check_all(
    const IntBitset *set,
    const uint8_t *model,
    size_t n
) {
    assert(int_bitset_validate(set));
    assert(int_bitset_count(set)==model_count(model,n));

    for(size_t i=0;i<n;++i) {
        bool actual=false;
        assert(int_bitset_test(set,i,&actual));
        assert(actual==(model[i]!=0));
    }

    assert(int_bitset_any(set)==(model_count(model,n)>0));
    assert(int_bitset_none(set)==(model_count(model,n)==0));
    assert(int_bitset_all(set)==(model_count(model,n)==n));
}

static void test_boundaries(void) {
    IntBitset *set=int_bitset_create(70);
    assert(set!=NULL);

    const size_t indices[]={0,1,62,63,64,65,69};

    for(size_t i=0;i<sizeof indices/sizeof indices[0];++i) {
        assert(int_bitset_set(set,indices[i]));
    }

    assert(int_bitset_count(set)==7);

    size_t next=0;
    assert(int_bitset_find_next_set(set,63,&next)&&next==63);
    assert(int_bitset_find_next_set(set,64,&next)&&next==64);
    assert(int_bitset_find_next_set(set,66,&next)&&next==69);
    assert(!int_bitset_find_next_set(set,69+1,&next));

    int_bitset_flip_all(set);
    assert(int_bitset_count(set)==63);
    assert(int_bitset_validate(set));

    int_bitset_set_all(set);
    assert(int_bitset_all(set));
    assert(int_bitset_count(set)==70);

    int_bitset_free(set);
}

static void test_bitwise(void) {
    IntBitset *a=int_bitset_create(130);
    IntBitset *b=int_bitset_create(130);
    IntBitset *out=int_bitset_create(130);

    assert(a&&b&&out);

    for(size_t i=0;i<130;i+=2)assert(int_bitset_set(a,i));
    for(size_t i=0;i<130;i+=3)assert(int_bitset_set(b,i));

    assert(int_bitset_and(out,a,b));

    for(size_t i=0;i<130;++i) {
        bool v=false;
        assert(int_bitset_test(out,i,&v));
        assert(v==(i%6==0));
    }

    assert(int_bitset_or(out,a,b));
    assert(int_bitset_validate(out));

    assert(int_bitset_xor(out,a,b));
    assert(int_bitset_validate(out));

    assert(int_bitset_not(out,a));
    assert(int_bitset_count(out)==65);

    int_bitset_free(out);
    int_bitset_free(b);
    int_bitset_free(a);
}

static void test_randomized(void) {
    enum { N=257, OPS=50000 };

    IntBitset *set=int_bitset_create(N);
    uint8_t model[N];
    memset(model,0,sizeof model);

    assert(set!=NULL);

    uint32_t rng=0x64A12345u;

    for(int op=0;op<OPS;++op) {
        const size_t i=next_rng(&rng)%N;
        const unsigned kind=next_rng(&rng)%5U;

        if(kind==0) {
            assert(int_bitset_set(set,i));
            model[i]=1;
        } else if(kind==1) {
            assert(int_bitset_clear(set,i));
            model[i]=0;
        } else if(kind==2) {
            assert(int_bitset_flip(set,i));
            model[i]^=1U;
        } else if(kind==3) {
            size_t start=next_rng(&rng)%N;
            size_t expected=N;

            for(size_t j=start;j<N;++j) {
                if(model[j]) {
                    expected=j;
                    break;
                }
            }

            size_t actual=0;
            const bool found=int_bitset_find_next_set(
                set,start,&actual
            );

            assert(found==(expected!=N));
            if(found)assert(actual==expected);
        } else {
            bool actual=false;
            assert(int_bitset_test(set,i,&actual));
            assert(actual==(model[i]!=0));
        }

        if((op%997)==0)check_all(set,model,N);
    }

    check_all(set,model,N);
    int_bitset_free(set);
}

int main(void) {
    IntBitset *empty=int_bitset_create(0);
    assert(empty!=NULL);
    assert(int_bitset_validate(empty));
    assert(int_bitset_all(empty));
    int_bitset_free(empty);

    test_boundaries();
    test_bitwise();
    test_randomized();

    puts("Bitset tests passed");
    return 0;
}
