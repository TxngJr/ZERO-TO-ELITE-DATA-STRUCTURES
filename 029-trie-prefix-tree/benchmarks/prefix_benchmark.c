#include "byte_trie.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const int n = 30000;

    ByteTrie *trie = byte_trie_create();
    char **words = calloc((size_t)n,sizeof *words);

    if (trie==NULL || words==NULL) return 1;

    for (int i=0;i<n;++i) {
        words[i]=malloc(32);
        if(words[i]==NULL)return 1;

        snprintf(words[i],32,"group/%02d/item/%05d",i%100,i);

        if(!byte_trie_insert(
            trie,
            (const unsigned char *)words[i],
            strlen(words[i])
        )) return 1;
    }

    const char *prefix="group/42/";
    const size_t prefix_len=strlen(prefix);

    struct timespec a,b;

    timespec_get(&a,TIME_UTC);
    const size_t trie_count=byte_trie_count_prefix(
        trie,
        (const unsigned char *)prefix,
        prefix_len
    );
    timespec_get(&b,TIME_UTC);
    const double trie_time=elapsed(a,b);

    timespec_get(&a,TIME_UTC);
    size_t linear_count=0;
    for(int i=0;i<n;++i) {
        if(strncmp(words[i],prefix,prefix_len)==0) ++linear_count;
    }
    timespec_get(&b,TIME_UTC);
    const double linear_time=elapsed(a,b);

    printf("n=%d matches=%zu trie_seconds=%.9f linear_seconds=%.9f\n",
           n,trie_count,trie_time,linear_time);

    if(trie_count!=linear_count)return 1;

    puts("This count_prefix implementation scans the matched subtree; cached counts could change the trade-off.");

    for(int i=0;i<n;++i)free(words[i]);
    free(words);
    byte_trie_free(trie);
    return 0;
}
