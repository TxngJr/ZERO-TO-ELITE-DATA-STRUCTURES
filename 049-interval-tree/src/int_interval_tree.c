#include "int_interval_tree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct IntervalNode {
    int64_t low;
    int64_t high;
    int64_t max_high;
    int height;
    struct IntervalNode *left;
    struct IntervalNode *right;
} IntervalNode;

struct IntIntervalTree {
    IntervalNode *root;
    size_t size;
};

static int node_height(const IntervalNode *node) {
    return node==NULL?0:node->height;
}

static int64_t node_max_high(const IntervalNode *node) {
    return node==NULL?INT64_MIN:node->max_high;
}

static int max_int(int a,int b) {
    return a>b?a:b;
}

static int64_t max_i64(int64_t a,int64_t b) {
    return a>b?a:b;
}

static void refresh(IntervalNode *node) {
    if(node==NULL)return;

    node->height=1+max_int(
        node_height(node->left),
        node_height(node->right)
    );

    node->max_high=max_i64(
        node->high,
        max_i64(
            node_max_high(node->left),
            node_max_high(node->right)
        )
    );
}

static int key_compare(
    int64_t low,
    int64_t high,
    const IntervalNode *node
) {
    if(low<node->low)return -1;
    if(low>node->low)return 1;
    if(high<node->high)return -1;
    if(high>node->high)return 1;
    return 0;
}

static IntervalNode *rotate_right(IntervalNode *y) {
    IntervalNode *x=y->left;
    IntervalNode *middle=x->right;

    x->right=y;
    y->left=middle;

    refresh(y);
    refresh(x);
    return x;
}

static IntervalNode *rotate_left(IntervalNode *x) {
    IntervalNode *y=x->right;
    IntervalNode *middle=y->left;

    y->left=x;
    x->right=middle;

    refresh(x);
    refresh(y);
    return y;
}

static IntervalNode *rebalance(IntervalNode *node) {
    if(node==NULL)return NULL;

    refresh(node);

    const int balance=
        node_height(node->left)-
        node_height(node->right);

    if(balance>1) {
        if(node_height(node->left->left)<
           node_height(node->left->right)) {
            node->left=rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if(balance<-1) {
        if(node_height(node->right->right)<
           node_height(node->right->left)) {
            node->right=rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

IntIntervalTree *int_interval_tree_create(void) {
    return calloc(1,sizeof(IntIntervalTree));
}

static void free_nodes(IntervalNode *node) {
    if(node==NULL)return;
    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}

void int_interval_tree_free(IntIntervalTree *tree) {
    if(tree==NULL)return;
    free_nodes(tree->root);
    free(tree);
}

size_t int_interval_tree_size(const IntIntervalTree *tree) {
    return tree==NULL?0:tree->size;
}

static IntervalNode *insert_rec(
    IntervalNode *node,
    int64_t low,
    int64_t high,
    bool *inserted
) {
    if(node==NULL) {
        IntervalNode *created=calloc(1,sizeof *created);
        if(created==NULL)return NULL;

        created->low=low;
        created->high=high;
        created->max_high=high;
        created->height=1;
        *inserted=true;
        return created;
    }

    const int cmp=key_compare(low,high,node);

    if(cmp==0)return node;

    if(cmp<0) {
        const bool before=*inserted;
        IntervalNode *next=insert_rec(
            node->left,low,high,inserted
        );

        if(!before && !*inserted && node->left==NULL) {
            return node;
        }

        if(*inserted)node->left=next;
    } else {
        const bool before=*inserted;
        IntervalNode *next=insert_rec(
            node->right,low,high,inserted
        );

        if(!before && !*inserted && node->right==NULL) {
            return node;
        }

        if(*inserted)node->right=next;
    }

    return *inserted?rebalance(node):node;
}

bool int_interval_tree_insert(
    IntIntervalTree *tree,
    int64_t low,
    int64_t high
) {
    if(tree==NULL||low>=high||tree->size==SIZE_MAX) {
        return false;
    }

    bool inserted=false;

    IntervalNode *root=insert_rec(
        tree->root,low,high,&inserted
    );

    if(!inserted)return false;

    tree->root=root;
    ++tree->size;
    return true;
}

bool int_interval_tree_contains(
    const IntIntervalTree *tree,
    int64_t low,
    int64_t high
) {
    if(tree==NULL||low>=high)return false;

    const IntervalNode *node=tree->root;

    while(node!=NULL) {
        const int cmp=key_compare(low,high,node);

        if(cmp==0)return true;
        node=cmp<0?node->left:node->right;
    }

    return false;
}

static IntervalNode *min_node(IntervalNode *node) {
    while(node->left!=NULL)node=node->left;
    return node;
}

static IntervalNode *remove_rec(
    IntervalNode *node,
    int64_t low,
    int64_t high,
    bool *removed
) {
    if(node==NULL)return NULL;

    const int cmp=key_compare(low,high,node);

    if(cmp<0) {
        node->left=remove_rec(
            node->left,low,high,removed
        );
    } else if(cmp>0) {
        node->right=remove_rec(
            node->right,low,high,removed
        );
    } else {
        *removed=true;

        if(node->left==NULL||node->right==NULL) {
            IntervalNode *child=
                node->left!=NULL?node->left:node->right;
            free(node);
            return child;
        }

        IntervalNode *successor=min_node(node->right);
        node->low=successor->low;
        node->high=successor->high;

        bool ignored=false;
        node->right=remove_rec(
            node->right,
            successor->low,
            successor->high,
            &ignored
        );
    }

    return rebalance(node);
}

bool int_interval_tree_remove(
    IntIntervalTree *tree,
    int64_t low,
    int64_t high
) {
    if(tree==NULL||low>=high)return false;

    bool removed=false;

    tree->root=remove_rec(
        tree->root,low,high,&removed
    );

    if(!removed)return false;

    --tree->size;
    return true;
}

static bool overlaps(
    int64_t a_low,
    int64_t a_high,
    int64_t b_low,
    int64_t b_high
) {
    return a_low<b_high && b_low<a_high;
}

bool int_interval_tree_find_overlap(
    const IntIntervalTree *tree,
    int64_t query_low,
    int64_t query_high,
    IntInterval *out_interval
) {
    if(tree==NULL||out_interval==NULL||
       query_low>=query_high) {
        return false;
    }

    const IntervalNode *node=tree->root;

    while(node!=NULL) {
        if(overlaps(
                node->low,node->high,
                query_low,query_high
            )) {
            *out_interval=(IntInterval){
                .low=node->low,
                .high=node->high
            };
            return true;
        }

        if(node->left!=NULL &&
           node->left->max_high>query_low) {
            node=node->left;
        } else {
            node=node->right;
        }
    }

    return false;
}

static size_t count_overlaps_rec(
    const IntervalNode *node,
    int64_t query_low,
    int64_t query_high
) {
    if(node==NULL||node->max_high<=query_low)return 0;

    size_t count=0;

    if(node->left!=NULL &&
       node->left->max_high>query_low) {
        count+=count_overlaps_rec(
            node->left,query_low,query_high
        );
    }

    if(node->low<query_high) {
        if(overlaps(
                node->low,node->high,
                query_low,query_high
            )) {
            ++count;
        }

        count+=count_overlaps_rec(
            node->right,query_low,query_high
        );
    }

    return count;
}

bool int_interval_tree_count_overlaps(
    const IntIntervalTree *tree,
    int64_t query_low,
    int64_t query_high,
    size_t *out_count
) {
    if(tree==NULL||out_count==NULL||
       query_low>=query_high) {
        return false;
    }

    *out_count=count_overlaps_rec(
        tree->root,query_low,query_high
    );
    return true;
}

typedef struct {
    bool ok;
    size_t count;
    int height;
    int64_t max_high;
    bool has_key;
    int64_t min_low;
    int64_t min_high;
    int64_t max_low;
    int64_t max_key_high;
} ValidateResult;

static bool pair_less(
    int64_t a_low,int64_t a_high,
    int64_t b_low,int64_t b_high
) {
    return a_low<b_low ||
           (a_low==b_low&&a_high<b_high);
}

static ValidateResult validate_rec(
    const IntervalNode *node
) {
    if(node==NULL) {
        return (ValidateResult){
            .ok=true,
            .count=0,
            .height=0,
            .max_high=INT64_MIN,
            .has_key=false
        };
    }

    if(node->low>=node->high) {
        return (ValidateResult){.ok=false};
    }

    ValidateResult left=validate_rec(node->left);
    ValidateResult right=validate_rec(node->right);

    if(!left.ok||!right.ok) {
        return (ValidateResult){.ok=false};
    }

    if(left.has_key &&
       !pair_less(
           left.max_low,left.max_key_high,
           node->low,node->high
       )) {
        return (ValidateResult){.ok=false};
    }

    if(right.has_key &&
       !pair_less(
           node->low,node->high,
           right.min_low,right.min_high
       )) {
        return (ValidateResult){.ok=false};
    }

    const int expected_height=
        1+max_int(left.height,right.height);

    const int balance=left.height-right.height;

    const int64_t expected_max=max_i64(
        node->high,
        max_i64(left.max_high,right.max_high)
    );

    if(node->height!=expected_height||
       balance<-1||balance>1||
       node->max_high!=expected_max) {
        return (ValidateResult){.ok=false};
    }

    ValidateResult result={
        .ok=true,
        .count=left.count+right.count+1,
        .height=expected_height,
        .max_high=expected_max,
        .has_key=true,
        .min_low=node->low,
        .min_high=node->high,
        .max_low=node->low,
        .max_key_high=node->high
    };

    if(left.has_key) {
        result.min_low=left.min_low;
        result.min_high=left.min_high;
    }

    if(right.has_key) {
        result.max_low=right.max_low;
        result.max_key_high=right.max_key_high;
    }

    return result;
}

bool int_interval_tree_validate(
    const IntIntervalTree *tree
) {
    if(tree==NULL)return false;

    const ValidateResult result=
        validate_rec(tree->root);

    return result.ok&&result.count==tree->size;
}
