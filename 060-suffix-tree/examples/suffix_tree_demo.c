#include "byte_suffix_tree.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t text[]={'b','a','n','a','n','a'};

    ByteSuffixTree *tree=
        byte_suffix_tree_create(text,sizeof text);

    assert(tree!=NULL);

    const uint8_t pattern[]={'a','n','a'};
    size_t count=0;

    assert(byte_suffix_tree_count(
        tree,pattern,sizeof pattern,&count
    ));

    printf(
        "ana occurrences=%zu nodes=%zu\n",
        count,byte_suffix_tree_node_count(tree)
    );

    assert(count==2);
    assert(byte_suffix_tree_validate(tree));

    byte_suffix_tree_free(tree);
    return 0;
}
