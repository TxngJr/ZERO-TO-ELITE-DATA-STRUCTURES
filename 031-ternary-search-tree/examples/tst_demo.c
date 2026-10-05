#include "byte_tst.h"

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
    ByteTST *tree = byte_tst_create();
    assert(tree != NULL);

    const char *words[] = {"car","cat","dog","door"};

    for (size_t i = 0; i < 4; ++i) {
        assert(byte_tst_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    printf("prefix do count=%zu\n",
           byte_tst_count_prefix(
               tree,
               (const unsigned char *)"do",
               2
           ));

    assert(byte_tst_visit_prefix(
        tree,
        NULL,
        0,
        print_key,
        NULL
    ));

    byte_tst_free(tree);
    return 0;
}
