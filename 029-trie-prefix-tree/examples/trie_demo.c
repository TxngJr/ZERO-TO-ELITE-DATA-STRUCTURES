#include "byte_trie.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool print_key(
    const unsigned char *bytes,
    size_t length,
    void *context
) {
    (void)context;
    printf("%.*s\n", (int)length, (const char *)bytes);
    return true;
}

int main(void) {
    ByteTrie *trie = byte_trie_create();
    assert(trie != NULL);

    const char *words[] = {"app","apple","apt","cat"};

    for (size_t i = 0; i < 4; ++i) {
        assert(byte_trie_insert(
            trie,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    printf("prefix ap count=%zu\n",
           byte_trie_count_prefix(
               trie,
               (const unsigned char *)"ap",
               2
           ));

    puts("keys under ap:");
    assert(byte_trie_visit_prefix(
        trie,
        (const unsigned char *)"ap",
        2,
        print_key,
        NULL
    ));

    byte_trie_free(trie);
    return 0;
}
