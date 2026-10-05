#include "int_order_stat_tree.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={40,20,60,10,30,50,70};

    IntOrderStatTree *tree=int_order_stat_tree_create();
    assert(tree!=NULL);

    for(size_t i=0;i<sizeof values/sizeof values[0];++i) {
        assert(int_order_stat_tree_insert(tree,values[i]));
    }

    int64_t key=0;
    size_t rank=0;

    assert(int_order_stat_tree_select(tree,4,&key));
    assert(int_order_stat_tree_rank(tree,55,&rank));

    printf("select(4)=%" PRId64 " rank(55)=%zu\n",key,rank);

    assert(int_order_stat_tree_validate(tree));
    int_order_stat_tree_free(tree);
    return 0;
}
