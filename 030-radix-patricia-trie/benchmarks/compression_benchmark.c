#include "byte_radix_tree.h"
#include "byte_trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const int n=20000;

    ByteTrie *trie=byte_trie_create();
    ByteRadixTree *radix=byte_radix_tree_create();

    if(trie==NULL || radix==NULL)return 1;

    char key[128];

    for(int i=0;i<n;++i) {
        snprintf(
            key,
            sizeof key,
            "very/long/shared/path/with/many/unary/bytes/item/%05d",
            i
        );

        const size_t length=strlen(key);

        if(!byte_trie_insert(
                trie,
                (const unsigned char *)key,
                length
            )) return 1;

        if(!byte_radix_tree_insert(
                radix,
                (const unsigned char *)key,
                length
            )) return 1;
    }

    const char *probe=
        "very/long/shared/path/with/many/unary/bytes/item/15000";
    const size_t probe_length=strlen(probe);

    struct timespec a,b;

    timespec_get(&a,TIME_UTC);
    for(int i=0;i<100000;++i) {
        if(!byte_trie_contains(
                trie,
                (const unsigned char *)probe,
                probe_length
            )) return 1;
    }
    timespec_get(&b,TIME_UTC);
    const double trie_time=elapsed(a,b);

    timespec_get(&a,TIME_UTC);
    for(int i=0;i<100000;++i) {
        if(!byte_radix_tree_contains(
                radix,
                (const unsigned char *)probe,
                probe_length
            )) return 1;
    }
    timespec_get(&b,TIME_UTC);
    const double radix_time=elapsed(a,b);

    printf(
        "n=%d radix_nodes=%zu trie_lookup=%.9f radix_lookup=%.9f\n",
        n,
        byte_radix_tree_node_count(radix),
        trie_time,
        radix_time
    );

    puts("Node compression reduces pointer hops, but label comparisons and allocation strategy affect timing.");

    byte_trie_free(trie);
    byte_radix_tree_free(radix);
    return 0;
}
