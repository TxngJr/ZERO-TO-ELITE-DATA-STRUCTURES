#include "int_cartesian_tree.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct IntCartesianTree {
    int64_t *values;
    size_t *parent;
    size_t *left;
    size_t *right;
    size_t count;
    size_t root;
};

static bool alloc_index_array(
    size_t count,
    size_t **out
) {
    if(count>SIZE_MAX/sizeof **out)return false;

    *out=malloc(count*sizeof **out);
    return *out!=NULL;
}

IntCartesianTree *int_cartesian_tree_create(
    const int64_t *values,
    size_t count
) {
    if(count>0&&values==NULL)return NULL;

    IntCartesianTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->count=count;
    tree->root=SIZE_MAX;

    if(count==0)return tree;

    if(count>SIZE_MAX/sizeof *tree->values) {
        free(tree);
        return NULL;
    }

    tree->values=malloc(count*sizeof *tree->values);

    if(tree->values==NULL||
       !alloc_index_array(count,&tree->parent)||
       !alloc_index_array(count,&tree->left)||
       !alloc_index_array(count,&tree->right)) {
        int_cartesian_tree_free(tree);
        return NULL;
    }

    memcpy(
        tree->values,
        values,
        count*sizeof *tree->values
    );

    for(size_t i=0;i<count;++i) {
        tree->parent[i]=SIZE_MAX;
        tree->left[i]=SIZE_MAX;
        tree->right[i]=SIZE_MAX;
    }

    size_t *stack=NULL;

    if(!alloc_index_array(count,&stack)) {
        int_cartesian_tree_free(tree);
        return NULL;
    }

    size_t top=0;

    for(size_t i=0;i<count;++i) {
        size_t last=SIZE_MAX;

        while(top>0 &&
              tree->values[stack[top-1]]>
                  tree->values[i]) {
            last=stack[--top];
        }

        if(top>0) {
            const size_t p=stack[top-1];

            tree->parent[i]=p;
            tree->right[p]=i;
        }

        if(last!=SIZE_MAX) {
            tree->parent[last]=i;
            tree->left[i]=last;
        }

        stack[top++]=i;
    }

    tree->root=stack[0];
    tree->parent[tree->root]=SIZE_MAX;

    free(stack);
    return tree;
}

void int_cartesian_tree_free(IntCartesianTree *tree) {
    if(tree==NULL)return;

    free(tree->values);
    free(tree->parent);
    free(tree->left);
    free(tree->right);
    free(tree);
}

size_t int_cartesian_tree_size(
    const IntCartesianTree *tree
) {
    return tree==NULL?0:tree->count;
}

bool int_cartesian_tree_root(
    const IntCartesianTree *tree,
    size_t *out_index
) {
    if(tree==NULL||out_index==NULL||tree->count==0) {
        return false;
    }

    *out_index=tree->root;
    return true;
}

static bool get_link(
    const IntCartesianTree *tree,
    const size_t *links,
    size_t index,
    size_t *out
) {
    if(tree==NULL||links==NULL||out==NULL||
       index>=tree->count) {
        return false;
    }

    *out=links[index];
    return true;
}

bool int_cartesian_tree_parent(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_parent
) {
    return get_link(tree,tree==NULL?NULL:tree->parent,
                    index,out_parent);
}

bool int_cartesian_tree_left(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_left
) {
    return get_link(tree,tree==NULL?NULL:tree->left,
                    index,out_left);
}

bool int_cartesian_tree_right(
    const IntCartesianTree *tree,
    size_t index,
    size_t *out_right
) {
    return get_link(tree,tree==NULL?NULL:tree->right,
                    index,out_right);
}

typedef struct {
    size_t index;
    size_t depth;
} DepthItem;

bool int_cartesian_tree_height(
    const IntCartesianTree *tree,
    size_t *out_edge_height
) {
    if(tree==NULL||out_edge_height==NULL||
       tree->count==0) {
        return false;
    }

    if(tree->count>SIZE_MAX/sizeof(DepthItem)) {
        return false;
    }

    DepthItem *stack=malloc(
        tree->count*sizeof *stack
    );

    if(stack==NULL)return false;

    size_t top=0;
    size_t max_depth=0;

    stack[top++]=(DepthItem){
        .index=tree->root,
        .depth=0
    };

    while(top>0) {
        const DepthItem item=stack[--top];

        if(item.depth>max_depth) {
            max_depth=item.depth;
        }

        const size_t l=tree->left[item.index];
        const size_t r=tree->right[item.index];

        if(l!=SIZE_MAX) {
            stack[top++]=(DepthItem){
                .index=l,
                .depth=item.depth+1
            };
        }

        if(r!=SIZE_MAX) {
            stack[top++]=(DepthItem){
                .index=r,
                .depth=item.depth+1
            };
        }
    }

    free(stack);
    *out_edge_height=max_depth;
    return true;
}

bool int_cartesian_tree_range_min(
    const IntCartesianTree *tree,
    size_t left,
    size_t right,
    size_t *out_index,
    int64_t *out_value
) {
    if(tree==NULL||out_index==NULL||
       out_value==NULL||left>=right||
       right>tree->count) {
        return false;
    }

    size_t node=tree->root;

    while(node!=SIZE_MAX) {
        if(node<left) {
            node=tree->right[node];
        } else if(node>=right) {
            node=tree->left[node];
        } else {
            *out_index=node;
            *out_value=tree->values[node];
            return true;
        }
    }

    return false;
}

bool int_cartesian_tree_validate(
    const IntCartesianTree *tree
) {
    if(tree==NULL)return false;

    if(tree->count==0) {
        return tree->root==SIZE_MAX&&
               tree->values==NULL&&
               tree->parent==NULL&&
               tree->left==NULL&&
               tree->right==NULL;
    }

    if(tree->root>=tree->count||
       tree->values==NULL||
       tree->parent==NULL||
       tree->left==NULL||
       tree->right==NULL||
       tree->parent[tree->root]!=SIZE_MAX) {
        return false;
    }

    bool *seen=calloc(tree->count,sizeof *seen);

    if(seen==NULL)return false;

    size_t *stack=NULL;

    if(!alloc_index_array(tree->count,&stack)) {
        free(seen);
        return false;
    }

    size_t top=0;
    size_t visited=0;
    stack[top++]=tree->root;

    while(top>0) {
        const size_t node=stack[--top];

        if(node>=tree->count||seen[node]) {
            free(stack);
            free(seen);
            return false;
        }

        seen[node]=true;
        ++visited;

        const size_t children[2]={
            tree->left[node],
            tree->right[node]
        };

        for(size_t c=0;c<2;++c) {
            const size_t child=children[c];

            if(child==SIZE_MAX)continue;

            if(child>=tree->count||
               tree->parent[child]!=node||
               tree->values[node]>tree->values[child]) {
                free(stack);
                free(seen);
                return false;
            }

            if(c==0&&child>=node) {
                free(stack);
                free(seen);
                return false;
            }

            if(c==1&&child<=node) {
                free(stack);
                free(seen);
                return false;
            }

            stack[top++]=child;
        }
    }

    if(visited!=tree->count) {
        free(stack);
        free(seen);
        return false;
    }

    /* Iterative inorder must reproduce indices 0..n-1 exactly. */
    top=0;
    size_t current=tree->root;
    size_t expected=0;

    while(current!=SIZE_MAX||top>0) {
        while(current!=SIZE_MAX) {
            stack[top++]=current;
            current=tree->left[current];
        }

        current=stack[--top];

        if(current!=expected++) {
            free(stack);
            free(seen);
            return false;
        }

        current=tree->right[current];
    }

    free(stack);
    free(seen);

    return expected==tree->count;
}
