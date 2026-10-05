#include "byte_suffix_automaton.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t sizes[]={1000,10000,100000};
    uint32_t rng=123456789u;

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];
        uint8_t *text=malloc(n);

        if(text==NULL)return 1;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            text[i]=(uint8_t)('a'+(rng%26U));
        }

        struct timespec a,b;
        timespec_get(&a,TIME_UTC);

        ByteSuffixAutomaton *sam=
            byte_suffix_automaton_create(text,n);

        timespec_get(&b,TIME_UTC);

        if(sam==NULL)return 1;

        printf(
            "n=%zu states=%zu build_seconds=%.9f\n",
            n,
            byte_suffix_automaton_state_count(sam),
            elapsed(a,b)
        );

        byte_suffix_automaton_free(sam);
        free(text);
    }

    return 0;
}
