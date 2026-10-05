#include "byte_suffix_automaton.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t naive_count(
    const uint8_t *text,size_t n,
    const uint8_t *pattern,size_t m
) {
    size_t count=0;

    for(size_t i=0;i+m<=n;++i) {
        if(memcmp(text+i,pattern,m)==0)++count;
    }

    return count;
}

static uint64_t naive_distinct(
    const uint8_t *text,size_t n
) {
    uint64_t total=0;

    for(size_t len=1;len<=n;++len) {
        for(size_t i=0;i+len<=n;++i) {
            bool first=true;

            for(size_t j=0;j<i;++j) {
                if(j+len<=n&&
                   memcmp(text+j,text+i,len)==0) {
                    first=false;
                    break;
                }
            }

            if(first)++total;
        }
    }

    return total;
}

static size_t naive_longest_repeated(
    const uint8_t *text,size_t n
) {
    size_t best=0;

    for(size_t len=1;len<=n;++len) {
        bool repeated=false;

        for(size_t i=0;i+len<=n&&!repeated;++i) {
            for(size_t j=i+1;j+len<=n;++j) {
                if(memcmp(text+i,text+j,len)==0) {
                    repeated=true;
                    break;
                }
            }
        }

        if(repeated)best=len;
    }

    return best;
}

static void test_known(void) {
    const uint8_t text[]={'a','b','a','b','a'};

    ByteSuffixAutomaton *sam=
        byte_suffix_automaton_create(text,sizeof text);

    assert(sam!=NULL);
    assert(byte_suffix_automaton_validate(sam));

    const uint8_t aba[]={'a','b','a'};
    size_t count=0;

    assert(byte_suffix_automaton_count(
        sam,aba,sizeof aba,&count
    ));
    assert(count==2);

    uint64_t distinct=0;
    assert(byte_suffix_automaton_distinct_substrings(
        sam,&distinct
    ));
    assert(distinct==naive_distinct(text,sizeof text));

    size_t repeated=0;
    assert(byte_suffix_automaton_longest_repeated(
        sam,&repeated
    ));
    assert(repeated==3);

    byte_suffix_automaton_free(sam);
}

static void test_binary(void) {
    const uint8_t text[]={0,1,0,1,0,255,0};
    const uint8_t pattern[]={0,1};

    ByteSuffixAutomaton *sam=
        byte_suffix_automaton_create(text,sizeof text);

    assert(sam!=NULL);
    assert(byte_suffix_automaton_validate(sam));

    size_t count=0;

    assert(byte_suffix_automaton_count(
        sam,pattern,sizeof pattern,&count
    ));
    assert(count==2);

    byte_suffix_automaton_free(sam);
}

static void test_randomized(void) {
    enum { ROUNDS=120, MAX_N=42, QUERIES=140 };

    uint32_t rng=0x61A12345u;

    for(int round=0;round<ROUNDS;++round) {
        const size_t n=1+(next_rng(&rng)%MAX_N);
        uint8_t *text=malloc(n);
        assert(text!=NULL);

        for(size_t i=0;i<n;++i) {
            text[i]=(uint8_t)(next_rng(&rng)%6U);
        }

        ByteSuffixAutomaton *sam=
            byte_suffix_automaton_create(text,n);

        assert(sam!=NULL);
        assert(byte_suffix_automaton_validate(sam));

        for(int q=0;q<QUERIES;++q) {
            uint8_t pattern[10];
            const size_t m=1+(next_rng(&rng)%sizeof pattern);

            for(size_t i=0;i<m;++i) {
                pattern[i]=(uint8_t)(next_rng(&rng)%6U);
            }

            const size_t expected=
                naive_count(text,n,pattern,m);

            size_t actual=0;

            assert(byte_suffix_automaton_count(
                sam,pattern,m,&actual
            ));

            assert(actual==expected);
            assert(byte_suffix_automaton_contains(
                sam,pattern,m
            )==(expected>0));
        }

        if(n<=18) {
            uint64_t actual_distinct=0;
            size_t actual_repeat=0;

            assert(byte_suffix_automaton_distinct_substrings(
                sam,&actual_distinct
            ));
            assert(byte_suffix_automaton_longest_repeated(
                sam,&actual_repeat
            ));

            assert(actual_distinct==naive_distinct(text,n));
            assert(actual_repeat==naive_longest_repeated(text,n));
        }

        byte_suffix_automaton_free(sam);
        free(text);
    }
}

int main(void) {
    ByteSuffixAutomaton *empty=
        byte_suffix_automaton_create(NULL,0);

    assert(empty!=NULL);
    assert(byte_suffix_automaton_validate(empty));
    byte_suffix_automaton_free(empty);

    test_known();
    test_binary();
    test_randomized();

    puts("Suffix Automaton tests passed");
    return 0;
}
