#include "dynamic_segment_tree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct DynamicNode {
    int64_t sum;
    struct DynamicNode *left;
    struct DynamicNode *right;
} DynamicNode;

struct DynamicSegmentTree {
    uint64_t domain_left;
    uint64_t domain_right;
    DynamicNode *root;
    size_t node_count;
};

DynamicSegmentTree *dynamic_segment_tree_create(
    uint64_t domain_left,
    uint64_t domain_right
) {
    if(domain_left>=domain_right)return NULL;

    DynamicSegmentTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->domain_left=domain_left;
    tree->domain_right=domain_right;
    return tree;
}

static void free_node(DynamicNode *node) {
    if(node==NULL)return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

void dynamic_segment_tree_free(DynamicSegmentTree *tree) {
    if(tree==NULL)return;
    free_node(tree->root);
    free(tree);
}

uint64_t dynamic_segment_tree_domain_left(
    const DynamicSegmentTree *tree
) {
    return tree==NULL?0:tree->domain_left;
}

uint64_t dynamic_segment_tree_domain_right(
    const DynamicSegmentTree *tree
) {
    return tree==NULL?0:tree->domain_right;
}

size_t dynamic_segment_tree_node_count(
    const DynamicSegmentTree *tree
) {
    return tree==NULL?0:tree->node_count;
}

static int64_t node_sum(const DynamicNode *node) {
    return node==NULL?0:node->sum;
}

static bool point_add_rec(
    DynamicSegmentTree *tree,
    DynamicNode **slot,
    uint64_t left,
    uint64_t right,
    uint64_t coordinate,
    int64_t delta
) {
    bool created=false;

    if(*slot==NULL) {
        if(tree->node_count==SIZE_MAX)return false;

        DynamicNode *node=calloc(1,sizeof *node);
        if(node==NULL)return false;

        *slot=node;
        created=true;
        ++tree->node_count;
    }

    DynamicNode *node=*slot;

    if(right-left==1U) {
        node->sum+=delta;
    } else {
        const uint64_t mid=
            left+(right-left)/2U;

        DynamicNode **child=
            coordinate<mid
                ? &node->left
                : &node->right;

        if(!point_add_rec(
                tree,
                child,
                coordinate<mid?left:mid,
                coordinate<mid?mid:right,
                coordinate,
                delta
            )) {
            if(created &&
               node->left==NULL &&
               node->right==NULL &&
               node->sum==0) {
                free(node);
                *slot=NULL;
                --tree->node_count;
            }
            return false;
        }

        node->sum=node_sum(node->left)+
                  node_sum(node->right);
    }

    if(node->sum==0 &&
       node->left==NULL &&
       node->right==NULL) {
        free(node);
        *slot=NULL;
        --tree->node_count;
    }

    return true;
}

bool dynamic_segment_tree_point_add(
    DynamicSegmentTree *tree,
    uint64_t coordinate,
    int64_t delta
) {
    if(tree==NULL ||
       coordinate<tree->domain_left ||
       coordinate>=tree->domain_right) {
        return false;
    }

    if(delta==0)return true;

    return point_add_rec(
        tree,
        &tree->root,
        tree->domain_left,
        tree->domain_right,
        coordinate,
        delta
    );
}

static int64_t range_sum_rec(
    const DynamicNode *node,
    uint64_t left,
    uint64_t right,
    uint64_t ql,
    uint64_t qr
) {
    if(node==NULL||qr<=left||right<=ql)return 0;

    if(ql<=left&&right<=qr)return node->sum;

    const uint64_t mid=left+(right-left)/2U;

    return range_sum_rec(
               node->left,left,mid,ql,qr
           )+
           range_sum_rec(
               node->right,mid,right,ql,qr
           );
}

bool dynamic_segment_tree_range_sum(
    const DynamicSegmentTree *tree,
    uint64_t left,
    uint64_t right,
    int64_t *out_sum
) {
    if(tree==NULL||out_sum==NULL||
       left>=right||
       left<tree->domain_left||
       right>tree->domain_right) {
        return false;
    }

    *out_sum=range_sum_rec(
        tree->root,
        tree->domain_left,
        tree->domain_right,
        left,
        right
    );
    return true;
}

bool dynamic_segment_tree_point_get(
    const DynamicSegmentTree *tree,
    uint64_t coordinate,
    int64_t *out_value
) {
    if(tree==NULL||out_value==NULL||
       coordinate<tree->domain_left||
       coordinate>=tree->domain_right) {
        return false;
    }

    if(coordinate==UINT64_MAX)return false;

    return dynamic_segment_tree_range_sum(
        tree,
        coordinate,
        coordinate+1U,
        out_value
    );
}

static bool validate_rec(
    const DynamicNode *node,
    uint64_t left,
    uint64_t right,
    size_t *count,
    int64_t *out_sum
) {
    if(node==NULL) {
        *out_sum=0;
        return true;
    }

    ++*count;

    if(right-left==1U) {
        if(node->left!=NULL||node->right!=NULL)return false;
        if(node->sum==0)return false;

        *out_sum=node->sum;
        return true;
    }

    const uint64_t mid=left+(right-left)/2U;

    int64_t left_sum=0;
    int64_t right_sum=0;

    if(!validate_rec(
            node->left,left,mid,count,&left_sum
        )||
       !validate_rec(
            node->right,mid,right,count,&right_sum
        )) {
        return false;
    }

    if(node->left==NULL&&
       node->right==NULL&&
       node->sum==0) {
        return false;
    }

    if(node->sum!=left_sum+right_sum)return false;

    *out_sum=node->sum;
    return true;
}

bool dynamic_segment_tree_validate(
    const DynamicSegmentTree *tree
) {
    if(tree==NULL ||
       tree->domain_left>=tree->domain_right) {
        return false;
    }

    size_t count=0;
    int64_t sum=0;

    if(!validate_rec(
            tree->root,
            tree->domain_left,
            tree->domain_right,
            &count,
            &sum
        )) {
        return false;
    }

    return count==tree->node_count;
}
