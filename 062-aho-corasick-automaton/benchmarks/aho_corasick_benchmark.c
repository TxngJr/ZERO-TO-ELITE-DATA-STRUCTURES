#include "byte_aho_corasick.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    enum { PATTERNS=2000, PATTERN_LENGTH=12 };
    uint32_t rng=123456789u;

    BytePattern *patterns=malloc(
        PATTERNS*sizeof *patterns
    );
    uint8_t *storage=malloc(
        PATTERNS*PATTERN_LENGTH
    );

    if(patterns==NULL||storage==NULL)return 1;

    for(size_t p=0;p<PATTERNS;++p) {
        uint8_t *bytes=storage+p*PATTERN_LENGTH;

        for(size_t i=0;i<PATTERN_LENGTH;++i) {
            rng=rng*1664525u+1013904223u;
            bytes[i]=(uint8_t)('a'+(rng%26U));
        }

        patterns[p]=(BytePattern){
            .bytes=bytes,
            .length=PATTERN_LENGTH,
            .id=(uint64_t)p
        };
    }

    struct timespec build_a,build_b;
    timespec_get(&build_a,TIME_UTC);

    ByteAhoCorasick *ac=byte_aho_corasick_create(
        patterns,PATTERNS
    );

    timespec_get(&build_b,TIME_UTC);

    if(ac==NULL)return 1;

    const size_t text_length=1000000;
    uint8_t *text=malloc(text_length);

    if(text==NULL)return 1;

    for(size_t i=0;i<text_length;++i) {
        rng=rng*1664525u+1013904223u;
        text[i]=(uint8_t)('a'+(rng%26U));
    }

    struct timespec scan_a,scan_b;
    size_t matches=0;

    timespec_get(&scan_a,TIME_UTC);

    if(!byte_aho_corasick_count_matches(
            ac,text,text_length,&matches
        )) {
        return 1;
    }

    timespec_get(&scan_b,TIME_UTC);

    printf(
        "patterns=%d states=%zu text=%zu matches=%zu build=%.9f scan=%.9f\n",
        PATTERNS,
        byte_aho_corasick_state_count(ac),
        text_length,
        matches,
        elapsed(build_a,build_b),
        elapsed(scan_a,scan_b)
    );

    free(text);
    byte_aho_corasick_free(ac);
    free(storage);
    free(patterns);
    return 0;
}
