#include "int_xor_trie.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    IntXorTrie *trie=int_xor_trie_create();
    assert(trie!=NULL);

    const uint64_t values[]={3,5,14,5};

    for(size_t i=0;i<4;++i) {
        assert(int_xor_trie_insert(trie,values[i]));
    }

    uint64_t value=0,score=0;

    assert(int_xor_trie_max_xor(
        trie,6,&value,&score
    ));

    printf(
        "max partner=%" PRIu64 " xor=%" PRIu64 " size=%zu\n",
        value,score,int_xor_trie_size(trie)
    );

    assert(int_xor_trie_validate(trie));
    int_xor_trie_free(trie);
    return 0;
}
