#include "byte_suffix_tree.h"

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

static void test_banana(void) {
    const uint8_t text[]={'b','a','n','a','n','a'};
    const uint8_t ana[]={'a','n','a'};

    ByteSuffixTree *tree=
        byte_suffix_tree_create(text,sizeof text);

    assert(tree!=NULL);
    assert(byte_suffix_tree_validate(tree));

    size_t count=0;
    size_t positions[4];
    size_t written=0;

    assert(byte_suffix_tree_count(
        tree,ana,sizeof ana,&count
    ));
    assert(count==2);

    assert(byte_suffix_tree_report(
        tree,ana,sizeof ana,
        positions,4,&written
    ));
    assert(written==2);

    bool has1=false,has3=false;

    for(size_t i=0;i<written;++i) {
        if(positions[i]==1)has1=true;
        if(positions[i]==3)has3=true;
    }

    assert(has1&&has3);

    const uint8_t absent[]={'x'};
    assert(!byte_suffix_tree_contains(
        tree,absent,sizeof absent
    ));

    byte_suffix_tree_free(tree);
}

static void test_binary(void) {
    const uint8_t text[]={0,1,0,1,0,255,0};
    const uint8_t pattern[]={0,1};

    ByteSuffixTree *tree=
        byte_suffix_tree_create(text,sizeof text);

    assert(tree!=NULL);
    assert(byte_suffix_tree_validate(tree));

    size_t count=0;

    assert(byte_suffix_tree_count(
        tree,pattern,sizeof pattern,&count
    ));
    assert(count==2);

    byte_suffix_tree_free(tree);
}

static void test_repeated(void) {
    enum { N=180 };

    uint8_t text[N];

    for(size_t i=0;i<N;++i)text[i]='a';

    ByteSuffixTree *tree=byte_suffix_tree_create(text,N);

    assert(tree!=NULL);
    assert(byte_suffix_tree_validate(tree));

    const uint8_t pattern[]={'a','a','a','a'};
    size_t count=0;

    assert(byte_suffix_tree_count(
        tree,pattern,sizeof pattern,&count
    ));

    assert(count==N-sizeof pattern+1);
    byte_suffix_tree_free(tree);
}

static void test_randomized(void) {
    enum { ROUNDS=80, MAX_N=120 };

    uint32_t rng=0x60A12345u;

    for(int round=0;round<ROUNDS;++round) {
        const size_t n=1+(next_rng(&rng)%MAX_N);
        uint8_t *text=malloc(n);

        assert(text!=NULL);

        for(size_t i=0;i<n;++i) {
            text[i]=(uint8_t)(next_rng(&rng)%6U);
        }

        ByteSuffixTree *tree=byte_suffix_tree_create(text,n);

        assert(tree!=NULL);
        assert(byte_suffix_tree_validate(tree));

        for(int q=0;q<100;++q) {
            uint8_t pattern[10];
            const size_t m=1+(next_rng(&rng)%sizeof pattern);

            for(size_t i=0;i<m;++i) {
                pattern[i]=(uint8_t)(next_rng(&rng)%6U);
            }

            const size_t expected=
                naive_count(text,n,pattern,m);

            size_t actual=0;

            assert(byte_suffix_tree_count(
                tree,pattern,m,&actual
            ));

            assert(actual==expected);
            assert(byte_suffix_tree_contains(
                tree,pattern,m
            )==(expected>0));

            if(expected>0&&expected<=n) {
                size_t *positions=malloc(
                    expected*sizeof *positions
                );
                assert(positions!=NULL);

                size_t written=0;

                assert(byte_suffix_tree_report(
                    tree,pattern,m,
                    positions,expected,&written
                ));

                assert(written==expected);

                for(size_t i=0;i<written;++i) {
                    assert(positions[i]+m<=n);
                    assert(memcmp(
                        text+positions[i],pattern,m
                    )==0);
                }

                free(positions);
            }
        }

        byte_suffix_tree_free(tree);
        free(text);
    }
}

int main(void) {
    ByteSuffixTree *empty=
        byte_suffix_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(byte_suffix_tree_validate(empty));
    byte_suffix_tree_free(empty);

    test_banana();
    test_binary();
    test_repeated();
    test_randomized();

    puts("Suffix Tree tests passed");
    return 0;
}
