#include "int_fenwick_tree.h"

#include <stdint.h>
#include <stdlib.h>

struct IntFenwickTree {
    size_t count;
    int64_t *tree;
};

static size_t lowbit(size_t value) {
    return value & (~value + (size_t)1);
}

IntFenwickTree *int_fenwick_tree_create(
    const int64_t *values,
    size_t count
) {
    if(count>0&&values==NULL)return NULL;

    IntFenwickTree *fenwick=calloc(1,sizeof *fenwick);
    if(fenwick==NULL)return NULL;

    fenwick->count=count;

    if(count==0)return fenwick;

    if(count==SIZE_MAX ||
       count+1>SIZE_MAX/sizeof *fenwick->tree) {
        free(fenwick);
        return NULL;
    }

    fenwick->tree=calloc(
        count+1,
        sizeof *fenwick->tree
    );

    if(fenwick->tree==NULL) {
        free(fenwick);
        return NULL;
    }

    for(size_t i=1;i<=count;++i) {
        fenwick->tree[i]+=values[i-1];

        const size_t step=lowbit(i);

        if(step<=count-i) {
            const size_t parent=i+step;
            fenwick->tree[parent]+=fenwick->tree[i];
        }
    }

    return fenwick;
}

void int_fenwick_tree_free(IntFenwickTree *tree) {
    if(tree==NULL)return;
    free(tree->tree);
    free(tree);
}

size_t int_fenwick_tree_size(
    const IntFenwickTree *tree
) {
    return tree==NULL?0:tree->count;
}

bool int_fenwick_tree_point_add(
    IntFenwickTree *tree,
    size_t index,
    int64_t delta
) {
    if(tree==NULL||index>=tree->count)return false;

    size_t i=index+1;

    while(i<=tree->count) {
        tree->tree[i]+=delta;

        const size_t step=lowbit(i);

        if(step==0||step>tree->count-i)break;
        i+=step;
    }

    return true;
}

bool int_fenwick_tree_prefix_sum(
    const IntFenwickTree *tree,
    size_t end,
    int64_t *out_sum
) {
    if(tree==NULL||out_sum==NULL||end>tree->count) {
        return false;
    }

    int64_t sum=0;
    size_t i=end;

    while(i>0) {
        sum+=tree->tree[i];
        i-=lowbit(i);
    }

    *out_sum=sum;
    return true;
}

bool int_fenwick_tree_range_sum(
    const IntFenwickTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
) {
    if(tree==NULL||out_sum==NULL||
       left>=right||right>tree->count) {
        return false;
    }

    int64_t before=0;
    int64_t through=0;

    if(!int_fenwick_tree_prefix_sum(
            tree,left,&before
        )||
       !int_fenwick_tree_prefix_sum(
            tree,right,&through
        )) {
        return false;
    }

    *out_sum=through-before;
    return true;
}

bool int_fenwick_tree_point_get(
    const IntFenwickTree *tree,
    size_t index,
    int64_t *out_value
) {
    if(tree==NULL||out_value==NULL||
       index>=tree->count) {
        return false;
    }

    return int_fenwick_tree_range_sum(
        tree,index,index+1,out_value
    );
}

bool int_fenwick_tree_point_set(
    IntFenwickTree *tree,
    size_t index,
    int64_t value
) {
    if(tree==NULL||index>=tree->count)return false;

    int64_t old=0;

    if(!int_fenwick_tree_point_get(
            tree,index,&old
        )) {
        return false;
    }

    return int_fenwick_tree_point_add(
        tree,index,value-old
    );
}

bool int_fenwick_tree_validate(
    const IntFenwickTree *tree
) {
    if(tree==NULL)return false;

    if(tree->count==0)return tree->tree==NULL;
    if(tree->tree==NULL)return false;

    for(size_t i=1;i<=tree->count;++i) {
        const size_t length=lowbit(i);
        const size_t left=i-length;
        int64_t expected=0;

        for(size_t j=left;j<i;++j) {
            int64_t value=0;

            if(!int_fenwick_tree_point_get(
                    tree,j,&value
                )) {
                return false;
            }

            expected+=value;
        }

        if(tree->tree[i]!=expected)return false;
    }

    return true;
}
