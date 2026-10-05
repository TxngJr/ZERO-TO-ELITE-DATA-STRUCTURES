#include "byte_suffix_array.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t *g_text=NULL;
static size_t g_length=0;

static int naive_suffix_compare(const void *a,const void *b) {
    size_t i=*(const size_t *)a;
    size_t j=*(const size_t *)b;

    while(i<g_length&&j<g_length) {
        if(g_text[i]<g_text[j])return -1;
        if(g_text[i]>g_text[j])return 1;
        ++i;++j;
    }

    if(i==g_length&&j==g_length)return 0;
    return i==g_length?-1:1;
}

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
    const size_t expected_sa[]={5,3,1,0,4,2};
    const size_t expected_lcp[]={0,1,3,0,0,2};

    ByteSuffixArray *array=byte_suffix_array_create(text,6);
    assert(array!=NULL);
    assert(byte_suffix_array_validate(array));

    for(size_t r=0;r<6;++r) {
        size_t value=0;

        assert(byte_suffix_array_at(array,r,&value));
        assert(value==expected_sa[r]);

        assert(byte_suffix_array_lcp_at(array,r,&value));
        assert(value==expected_lcp[r]);
    }

    const uint8_t pattern[]={'a','n','a'};
    size_t count=0;
    size_t positions[4];
    size_t written=0;

    assert(byte_suffix_array_count(
        array,pattern,3,&count
    ));
    assert(count==2);

    assert(byte_suffix_array_report(
        array,pattern,3,
        positions,4,&written
    ));
    assert(written==2);

    byte_suffix_array_free(array);
}

static void test_binary(void) {
    const uint8_t text[]={0,1,0,1,0,255,0};
    const uint8_t pattern[]={0,1};

    ByteSuffixArray *array=
        byte_suffix_array_create(text,sizeof text);

    assert(array!=NULL);
    assert(byte_suffix_array_validate(array));

    size_t count=0;

    assert(byte_suffix_array_count(
        array,pattern,sizeof pattern,&count
    ));

    assert(count==2);
    byte_suffix_array_free(array);
}

static void test_randomized(void) {
    enum { ROUNDS=120, MAX_N=180 };

    uint32_t rng=0x59A12345u;

    for(int round=0;round<ROUNDS;++round) {
        const size_t n=1+(next_rng(&rng)%MAX_N);

        uint8_t *text=malloc(n);
        size_t *expected=malloc(n*sizeof *expected);

        assert(text!=NULL&&expected!=NULL);

        for(size_t i=0;i<n;++i) {
            text[i]=(uint8_t)(next_rng(&rng)%8U);
            expected[i]=i;
        }

        g_text=text;
        g_length=n;
        qsort(expected,n,sizeof *expected,naive_suffix_compare);

        ByteSuffixArray *array=byte_suffix_array_create(text,n);
        assert(array!=NULL);
        assert(byte_suffix_array_validate(array));

        for(size_t r=0;r<n;++r) {
            size_t actual=0;
            assert(byte_suffix_array_at(array,r,&actual));
            assert(actual==expected[r]);
        }

        for(int q=0;q<120;++q) {
            uint8_t pattern[12];
            const size_t m=1+(next_rng(&rng)%sizeof pattern);

            for(size_t i=0;i<m;++i) {
                pattern[i]=(uint8_t)(next_rng(&rng)%8U);
            }

            const size_t expected_count=
                naive_count(text,n,pattern,m);

            size_t actual_count=0;

            assert(byte_suffix_array_count(
                array,pattern,m,&actual_count
            ));

            assert(actual_count==expected_count);
            assert(byte_suffix_array_contains(
                array,pattern,m
            )==(expected_count>0));
        }

        byte_suffix_array_free(array);
        free(expected);
        free(text);
    }

    g_text=NULL;
    g_length=0;
}

int main(void) {
    ByteSuffixArray *empty=
        byte_suffix_array_create(NULL,0);

    assert(empty!=NULL);
    assert(byte_suffix_array_validate(empty));
    byte_suffix_array_free(empty);

    test_banana();
    test_binary();
    test_randomized();

    puts("Suffix Array tests passed");
    return 0;
}
