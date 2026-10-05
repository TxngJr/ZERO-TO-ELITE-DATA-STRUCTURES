#include "persistent_segment_tree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t left;
    size_t right;
    int64_t sum;
    int64_t min;
} PersistentNode;

struct PersistentSegmentTree {
    size_t count;

    PersistentNode *nodes;
    size_t node_count;
    size_t node_capacity;

    size_t *roots;
    size_t version_count;
    size_t version_capacity;
};

static int64_t min_i64(int64_t a,int64_t b) {
    return a<b?a:b;
}

static bool reserve_nodes(
    PersistentSegmentTree *tree,
    size_t needed
) {
    if(needed<=tree->node_capacity)return true;

    size_t capacity=
        tree->node_capacity==0?16:tree->node_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *tree->nodes)return false;

    PersistentNode *next=realloc(
        tree->nodes,
        capacity*sizeof *next
    );
    if(next==NULL)return false;

    tree->nodes=next;
    tree->node_capacity=capacity;
    return true;
}

static bool reserve_versions(
    PersistentSegmentTree *tree,
    size_t needed
) {
    if(needed<=tree->version_capacity)return true;

    size_t capacity=
        tree->version_capacity==0?8:tree->version_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *tree->roots)return false;

    size_t *next=realloc(
        tree->roots,
        capacity*sizeof *next
    );
    if(next==NULL)return false;

    tree->roots=next;
    tree->version_capacity=capacity;
    return true;
}

static bool append_node(
    PersistentSegmentTree *tree,
    PersistentNode node,
    size_t *out_index
) {
    if(tree->node_count>=SIZE_MAX-1)return false;

    const size_t index=tree->node_count+1;

    if(!reserve_nodes(tree,index+1))return false;

    tree->nodes[index]=node;
    tree->node_count=index;
    *out_index=index;
    return true;
}

static bool build_rec(
    PersistentSegmentTree *tree,
    const int64_t *values,
    size_t left,
    size_t right,
    size_t *out_node
) {
    if(right-left==1) {
        return append_node(
            tree,
            (PersistentNode){
                .left=0,
                .right=0,
                .sum=values[left],
                .min=values[left]
            },
            out_node
        );
    }

    const size_t old_count=tree->node_count;
    const size_t mid=left+(right-left)/2;

    size_t lc=0;
    size_t rc=0;

    if(!build_rec(tree,values,left,mid,&lc) ||
       !build_rec(tree,values,mid,right,&rc)) {
        tree->node_count=old_count;
        return false;
    }

    return append_node(
        tree,
        (PersistentNode){
            .left=lc,
            .right=rc,
            .sum=tree->nodes[lc].sum+
                 tree->nodes[rc].sum,
            .min=min_i64(
                tree->nodes[lc].min,
                tree->nodes[rc].min
            )
        },
        out_node
    );
}

PersistentSegmentTree *persistent_segment_tree_create(
    const int64_t *values,
    size_t count
) {
    if(count>0&&values==NULL)return NULL;
    if(count>(SIZE_MAX-1U)/2U)return NULL;

    PersistentSegmentTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->count=count;

    if(!reserve_versions(tree,1)) {
        persistent_segment_tree_free(tree);
        return NULL;
    }

    if(count==0) {
        tree->roots[0]=0;
        tree->version_count=1;
        return tree;
    }

    size_t root=0;

    if(!build_rec(tree,values,0,count,&root)) {
        persistent_segment_tree_free(tree);
        return NULL;
    }

    tree->roots[0]=root;
    tree->version_count=1;
    return tree;
}

void persistent_segment_tree_free(
    PersistentSegmentTree *tree
) {
    if(tree==NULL)return;
    free(tree->nodes);
    free(tree->roots);
    free(tree);
}

size_t persistent_segment_tree_size(
    const PersistentSegmentTree *tree
) {
    return tree==NULL?0:tree->count;
}

size_t persistent_segment_tree_version_count(
    const PersistentSegmentTree *tree
) {
    return tree==NULL?0:tree->version_count;
}

size_t persistent_segment_tree_node_count(
    const PersistentSegmentTree *tree
) {
    return tree==NULL?0:tree->node_count;
}

static bool update_rec(
    PersistentSegmentTree *tree,
    size_t old_node,
    size_t left,
    size_t right,
    size_t index,
    int64_t value,
    size_t *out_node
) {
    if(old_node==0||old_node>tree->node_count)return false;

    const PersistentNode old=tree->nodes[old_node];

    if(right-left==1) {
        return append_node(
            tree,
            (PersistentNode){
                .left=0,
                .right=0,
                .sum=value,
                .min=value
            },
            out_node
        );
    }

    const size_t mid=left+(right-left)/2;
    size_t lc=old.left;
    size_t rc=old.right;

    if(index<mid) {
        if(!update_rec(
                tree,old.left,left,mid,
                index,value,&lc
            )) {
            return false;
        }
    } else {
        if(!update_rec(
                tree,old.right,mid,right,
                index,value,&rc
            )) {
            return false;
        }
    }

    return append_node(
        tree,
        (PersistentNode){
            .left=lc,
            .right=rc,
            .sum=tree->nodes[lc].sum+
                 tree->nodes[rc].sum,
            .min=min_i64(
                tree->nodes[lc].min,
                tree->nodes[rc].min
            )
        },
        out_node
    );
}

bool persistent_segment_tree_point_set(
    PersistentSegmentTree *tree,
    size_t base_version,
    size_t index,
    int64_t value,
    size_t *out_new_version
) {
    if(tree==NULL||out_new_version==NULL||
       base_version>=tree->version_count||
       index>=tree->count||
       tree->version_count==SIZE_MAX) {
        return false;
    }

    if(!reserve_versions(
            tree,
            tree->version_count+1
        )) {
        return false;
    }

    const size_t old_node_count=tree->node_count;
    size_t new_root=0;

    if(!update_rec(
            tree,
            tree->roots[base_version],
            0,
            tree->count,
            index,
            value,
            &new_root
        )) {
        tree->node_count=old_node_count;
        return false;
    }

    const size_t version=tree->version_count;
    tree->roots[version]=new_root;
    ++tree->version_count;

    *out_new_version=version;
    return true;
}

typedef struct {
    int64_t sum;
    int64_t min;
    bool has_value;
} QueryResult;

static QueryResult query_rec(
    const PersistentSegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    size_t ql,
    size_t qr
) {
    if(node==0||qr<=left||right<=ql) {
        return (QueryResult){0};
    }

    if(ql<=left&&right<=qr) {
        return (QueryResult){
            .sum=tree->nodes[node].sum,
            .min=tree->nodes[node].min,
            .has_value=true
        };
    }

    const size_t mid=left+(right-left)/2;

    QueryResult a=query_rec(
        tree,
        tree->nodes[node].left,
        left,mid,ql,qr
    );
    QueryResult b=query_rec(
        tree,
        tree->nodes[node].right,
        mid,right,ql,qr
    );

    if(!a.has_value)return b;
    if(!b.has_value)return a;

    return (QueryResult){
        .sum=a.sum+b.sum,
        .min=min_i64(a.min,b.min),
        .has_value=true
    };
}

bool persistent_segment_tree_range_sum(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t left,
    size_t right,
    int64_t *out_sum
) {
    if(tree==NULL||out_sum==NULL||
       version>=tree->version_count||
       left>=right||right>tree->count) {
        return false;
    }

    QueryResult result=query_rec(
        tree,
        tree->roots[version],
        0,tree->count,left,right
    );

    if(!result.has_value)return false;

    *out_sum=result.sum;
    return true;
}

bool persistent_segment_tree_range_min(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t left,
    size_t right,
    int64_t *out_min
) {
    if(tree==NULL||out_min==NULL||
       version>=tree->version_count||
       left>=right||right>tree->count) {
        return false;
    }

    QueryResult result=query_rec(
        tree,
        tree->roots[version],
        0,tree->count,left,right
    );

    if(!result.has_value)return false;

    *out_min=result.min;
    return true;
}

bool persistent_segment_tree_point_get(
    const PersistentSegmentTree *tree,
    size_t version,
    size_t index,
    int64_t *out_value
) {
    if(tree==NULL||out_value==NULL||
       index>=tree->count) {
        return false;
    }

    return persistent_segment_tree_range_sum(
        tree,version,index,index+1,out_value
    );
}

static bool validate_version_rec(
    const PersistentSegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    int64_t *out_sum,
    int64_t *out_min
) {
    if(node==0||node>tree->node_count)return false;

    const PersistentNode *current=&tree->nodes[node];

    if(right-left==1) {
        if(current->left!=0||current->right!=0||
           current->sum!=current->min) {
            return false;
        }

        *out_sum=current->sum;
        *out_min=current->min;
        return true;
    }

    if(current->left==0||current->right==0||
       current->left>tree->node_count||
       current->right>tree->node_count) {
        return false;
    }

    const size_t mid=left+(right-left)/2;

    int64_t lsum=0,lmin=0,rsum=0,rmin=0;

    if(!validate_version_rec(
            tree,current->left,left,mid,&lsum,&lmin
        )||
       !validate_version_rec(
            tree,current->right,mid,right,&rsum,&rmin
        )) {
        return false;
    }

    const int64_t expected_sum=lsum+rsum;
    const int64_t expected_min=min_i64(lmin,rmin);

    if(current->sum!=expected_sum||
       current->min!=expected_min) {
        return false;
    }

    *out_sum=expected_sum;
    *out_min=expected_min;
    return true;
}

bool persistent_segment_tree_validate(
    const PersistentSegmentTree *tree
) {
    if(tree==NULL||tree->version_count==0||
       tree->roots==NULL) {
        return false;
    }

    if(tree->count==0) {
        if(tree->node_count!=0)return false;

        for(size_t v=0;v<tree->version_count;++v) {
            if(tree->roots[v]!=0)return false;
        }

        return true;
    }

    if(tree->nodes==NULL||tree->node_count==0)return false;

    for(size_t version=0;
        version<tree->version_count;
        ++version) {
        int64_t sum=0;
        int64_t min=0;

        if(!validate_version_rec(
                tree,
                tree->roots[version],
                0,
                tree->count,
                &sum,
                &min
            )) {
            return false;
        }
    }

    return true;
}
