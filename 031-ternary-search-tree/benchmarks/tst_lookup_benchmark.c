#include "byte_tst.h"
#include "byte_trie.h"
#include "byte_radix_tree.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const int n=20000;

    ByteTST *tst=byte_tst_create();
    ByteTrie *trie=byte_trie_create();
    ByteRadixTree *radix=byte_radix_tree_create();

    if(!tst||!trie||!radix)return 1;

    char key[64];

    for(int i=0;i<n;++i) {
        snprintf(key,sizeof key,"group/%02d/item/%05d",i%100,i);
        const size_t len=strlen(key);

        if(!byte_tst_insert(tst,(const unsigned char *)key,len))return 1;
        if(!byte_trie_insert(trie,(const unsigned char *)key,len))return 1;
        if(!byte_radix_tree_insert(radix,(const unsigned char *)key,len))return 1;
    }

    const char *probe="group/42/item/15042";
    const size_t len=strlen(probe);
    const int loops=100000;
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);
    for(int i=0;i<loops;++i)
        if(!byte_tst_contains(tst,(const unsigned char *)probe,len))return 1;
    timespec_get(&b,TIME_UTC);
    const double tst_s=elapsed(a,b);

    timespec_get(&a,TIME_UTC);
    for(int i=0;i<loops;++i)
        if(!byte_trie_contains(trie,(const unsigned char *)probe,len))return 1;
    timespec_get(&b,TIME_UTC);
    const double trie_s=elapsed(a,b);

    timespec_get(&a,TIME_UTC);
    for(int i=0;i<loops;++i)
        if(!byte_radix_tree_contains(radix,(const unsigned char *)probe,len))return 1;
    timespec_get(&b,TIME_UTC);
    const double radix_s=elapsed(a,b);

    printf("n=%d tst=%.9f trie=%.9f radix=%.9f\n",
           n,tst_s,trie_s,radix_s);

    puts("Timing depends on insertion order, allocation strategy, cache and workload.");

    byte_tst_free(tst);
    byte_trie_free(trie);
    byte_radix_tree_free(radix);
    return 0;
}
