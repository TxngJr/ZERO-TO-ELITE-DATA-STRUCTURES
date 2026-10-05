#include "int_xor_trie.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static uint64_t next_rng(uint64_t *state) {
    *state=*state*UINT64_C(6364136223846793005)+
        UINT64_C(1442695040888963407);
    return *state;
}

int main(void) {
    const size_t n=200000;
    const size_t queries=500000;

    IntXorTrie *trie=int_xor_trie_create();
    if(trie==NULL)return 1;

    uint64_t rng=UINT64_C(123456789);

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);

    for(size_t i=0;i<n;++i) {
        if(!int_xor_trie_insert(trie,next_rng(&rng))) {
            return 1;
        }
    }

    timespec_get(&b,TIME_UTC);
    const double build=elapsed(a,b);

    volatile uint64_t checksum=0;

    timespec_get(&a,TIME_UTC);

    for(size_t q=0;q<queries;++q) {
        uint64_t value=0,score=0;

        if(!int_xor_trie_max_xor(
                trie,next_rng(&rng),&value,&score
            )) {
            return 1;
        }

        checksum^=value^score;
    }

    timespec_get(&b,TIME_UTC);

    printf(
        "n=%zu nodes=%zu build=%.9f queries=%zu maxxor=%.9f checksum=%llu\n",
        n,
        int_xor_trie_node_count(trie),
        build,
        queries,
        elapsed(a,b),
        (unsigned long long)checksum
    );

    int_xor_trie_free(trie);
    return 0;
}
