#include "byte_aho_corasick.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t naive_match_count(
    const BytePattern *patterns,size_t pattern_count,
    const uint8_t *text,size_t text_length
) {
    size_t count=0;

    for(size_t p=0;p<pattern_count;++p) {
        for(size_t i=0;i+patterns[p].length<=text_length;++i) {
            if(memcmp(
                    text+i,
                    patterns[p].bytes,
                    patterns[p].length
                )==0) {
                ++count;
            }
        }
    }

    return count;
}

static bool naive_has_match(
    const BytePattern *patterns,size_t pattern_count,
    const uint8_t *text,size_t text_length,
    uint64_t id,size_t end
) {
    for(size_t p=0;p<pattern_count;++p) {
        if(patterns[p].id!=id||
           end<patterns[p].length) {
            continue;
        }

        const size_t start=end-patterns[p].length;

        if(end<=text_length&&
           memcmp(
               text+start,
               patterns[p].bytes,
               patterns[p].length
           )==0) {
            return true;
        }
    }

    return false;
}

static void test_classic(void) {
    const BytePattern patterns[]={
        {(const uint8_t *)"he",2,1},
        {(const uint8_t *)"she",3,2},
        {(const uint8_t *)"his",3,3},
        {(const uint8_t *)"hers",4,4}
    };

    const uint8_t text[]={'u','s','h','e','r','s'};

    ByteAhoCorasick *ac=byte_aho_corasick_create(
        patterns,4
    );
    assert(ac!=NULL);
    assert(byte_aho_corasick_validate(ac));

    size_t count=0;

    assert(byte_aho_corasick_count_matches(
        ac,text,sizeof text,&count
    ));

    assert(count==3);

    ByteACMatch matches[8];
    size_t written=0;

    assert(byte_aho_corasick_report(
        ac,text,sizeof text,
        matches,8,&written
    ));

    assert(written==3);

    for(size_t i=0;i<written;++i) {
        assert(naive_has_match(
            patterns,4,text,sizeof text,
            matches[i].pattern_id,
            matches[i].end_position
        ));
    }

    byte_aho_corasick_free(ac);
}

static void test_duplicates_and_binary(void) {
    const uint8_t p0[]={0,1};
    const uint8_t p1[]={0,1};
    const uint8_t p2[]={1,0};

    const BytePattern patterns[]={
        {p0,2,10},
        {p1,2,99},
        {p2,2,77}
    };

    const uint8_t text[]={0,1,0,1,0};

    ByteAhoCorasick *ac=byte_aho_corasick_create(
        patterns,3
    );

    assert(ac!=NULL);
    assert(byte_aho_corasick_validate(ac));

    size_t count=0;

    assert(byte_aho_corasick_count_matches(
        ac,text,sizeof text,&count
    ));

    assert(count==6);
    byte_aho_corasick_free(ac);
}

static void test_randomized(void) {
    enum {
        ROUNDS=90,
        PATTERNS=24,
        MAX_PATTERN=8,
        TEXT=180
    };

    uint32_t rng=0x62A12345u;

    for(int round=0;round<ROUNDS;++round) {
        BytePattern patterns[PATTERNS];
        uint8_t storage[PATTERNS][MAX_PATTERN];

        for(size_t p=0;p<PATTERNS;++p) {
            const size_t len=1+
                (next_rng(&rng)%MAX_PATTERN);

            for(size_t i=0;i<len;++i) {
                storage[p][i]=(uint8_t)(next_rng(&rng)%7U);
            }

            patterns[p]=(BytePattern){
                .bytes=storage[p],
                .length=len,
                .id=(uint64_t)(1000+p)
            };
        }

        ByteAhoCorasick *ac=byte_aho_corasick_create(
            patterns,PATTERNS
        );

        assert(ac!=NULL);
        assert(byte_aho_corasick_validate(ac));

        uint8_t text[TEXT];

        for(size_t i=0;i<TEXT;++i) {
            text[i]=(uint8_t)(next_rng(&rng)%7U);
        }

        const size_t expected=naive_match_count(
            patterns,PATTERNS,text,TEXT
        );

        size_t actual=0;

        assert(byte_aho_corasick_count_matches(
            ac,text,TEXT,&actual
        ));

        assert(actual==expected);

        ByteACMatch *matches=
            expected==0?NULL:malloc(expected*sizeof *matches);

        assert(expected==0||matches!=NULL);

        size_t written=0;

        assert(byte_aho_corasick_report(
            ac,text,TEXT,
            matches,expected,&written
        ));

        assert(written==expected);

        for(size_t i=0;i<written;++i) {
            assert(naive_has_match(
                patterns,PATTERNS,text,TEXT,
                matches[i].pattern_id,
                matches[i].end_position
            ));
        }

        free(matches);
        byte_aho_corasick_free(ac);
    }
}

int main(void) {
    assert(byte_aho_corasick_create(NULL,1)==NULL);

    test_classic();
    test_duplicates_and_binary();
    test_randomized();

    puts("Aho-Corasick tests passed");
    return 0;
}
