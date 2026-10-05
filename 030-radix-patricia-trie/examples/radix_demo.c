#include "byte_radix_tree.h"

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
    ByteRadixTree *tree = byte_radix_tree_create();
    assert(tree != NULL);

    const char *words[] = {"car","carpet","carbon","cat"};

    for (size_t i = 0; i < 4; ++i) {
        assert(byte_radix_tree_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    printf("size=%zu nodes=%zu prefix ca=%zu\n",
           byte_radix_tree_size(tree),
           byte_radix_tree_node_count(tree),
           byte_radix_tree_count_prefix(
               tree,
               (const unsigned char *)"ca",
               2
           ));

    assert(byte_radix_tree_visit_prefix(
        tree,
        (const unsigned char *)"car",
        3,
        print_key,
        NULL
    ));

    byte_radix_tree_free(tree);
    return 0;
}
