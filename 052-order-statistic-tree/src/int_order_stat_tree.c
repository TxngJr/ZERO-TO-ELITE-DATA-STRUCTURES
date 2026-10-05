#include "int_order_stat_tree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct OrderNode {
    int64_t key;
    int height;
    size_t subtree_size;
    struct OrderNode *left;
    struct OrderNode *right;
} OrderNode;

struct IntOrderStatTree {
    OrderNode *root;
    size_t size;
};

static int height_of(const OrderNode *node) {
    return node==NULL?0:node->height;
}

static size_t size_of(const OrderNode *node) {
    return node==NULL?0:node->subtree_size;
}

static int max_int(int a,int b) {
    return a>b?a:b;
}

static void refresh(OrderNode *node) {
    if(node==NULL)return;

    node->height=1+max_int(
        height_of(node->left),
        height_of(node->right)
    );

    node->subtree_size=
        size_of(node->left)+
        size_of(node->right)+1;
}

static int balance_factor(const OrderNode *node) {
    return node==NULL
        ?0
        :height_of(node->left)-height_of(node->right);
}

static OrderNode *rotate_right(OrderNode *y) {
    OrderNode *x=y->left;
    OrderNode *middle=x->right;

    x->right=y;
    y->left=middle;

    refresh(y);
    refresh(x);
    return x;
}

static OrderNode *rotate_left(OrderNode *x) {
    OrderNode *y=x->right;
    OrderNode *middle=y->left;

    y->left=x;
    x->right=middle;

    refresh(x);
    refresh(y);
    return y;
}

static OrderNode *rebalance(OrderNode *node) {
    if(node==NULL)return NULL;

    refresh(node);
    const int bf=balance_factor(node);

    if(bf>1) {
        if(balance_factor(node->left)<0) {
            node->left=rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if(bf<-1) {
        if(balance_factor(node->right)>0) {
            node->right=rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

static OrderNode *make_node(int64_t key) {
    OrderNode *node=calloc(1,sizeof *node);
    if(node==NULL)return NULL;

    node->key=key;
    node->height=1;
    node->subtree_size=1;
    return node;
}

IntOrderStatTree *int_order_stat_tree_create(void) {
    return calloc(1,sizeof(IntOrderStatTree));
}

static void free_nodes(OrderNode *node) {
    if(node==NULL)return;
    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}

void int_order_stat_tree_free(IntOrderStatTree *tree) {
    if(tree==NULL)return;
    free_nodes(tree->root);
    free(tree);
}

size_t int_order_stat_tree_size(
    const IntOrderStatTree *tree
) {
    return tree==NULL?0:tree->size;
}

bool int_order_stat_tree_contains(
    const IntOrderStatTree *tree,
    int64_t key
) {
    if(tree==NULL)return false;

    const OrderNode *node=tree->root;

    while(node!=NULL) {
        if(key==node->key)return true;
        node=key<node->key?node->left:node->right;
    }

    return false;
}

static OrderNode *insert_rec(
    OrderNode *node,
    int64_t key,
    int *status
) {
    if(node==NULL) {
        OrderNode *created=make_node(key);

        if(created==NULL) {
            *status=-1;
            return NULL;
        }

        *status=1;
        return created;
    }

    if(key==node->key) {
        *status=0;
        return node;
    }

    if(key<node->key) {
        OrderNode *next=insert_rec(
            node->left,key,status
        );

        if(*status<0)return node;
        node->left=next;
    } else {
        OrderNode *next=insert_rec(
            node->right,key,status
        );

        if(*status<0)return node;
        node->right=next;
    }

    return *status==1?rebalance(node):node;
}

bool int_order_stat_tree_insert(
    IntOrderStatTree *tree,
    int64_t key
) {
    if(tree==NULL||tree->size==SIZE_MAX)return false;

    int status=0;
    OrderNode *root=insert_rec(
        tree->root,key,&status
    );

    if(status!=1)return false;

    tree->root=root;
    ++tree->size;
    return true;
}

static OrderNode *minimum_node(OrderNode *node) {
    while(node->left!=NULL)node=node->left;
    return node;
}

static OrderNode *remove_rec(
    OrderNode *node,
    int64_t key,
    bool *removed
) {
    if(node==NULL)return NULL;

    if(key<node->key) {
        node->left=remove_rec(
            node->left,key,removed
        );
    } else if(key>node->key) {
        node->right=remove_rec(
            node->right,key,removed
        );
    } else {
        *removed=true;

        if(node->left==NULL||node->right==NULL) {
            OrderNode *child=
                node->left!=NULL?node->left:node->right;
            free(node);
            return child;
        }

        OrderNode *successor=minimum_node(node->right);
        node->key=successor->key;

        bool ignored=false;
        node->right=remove_rec(
            node->right,
            successor->key,
            &ignored
        );
    }

    return rebalance(node);
}

bool int_order_stat_tree_remove(
    IntOrderStatTree *tree,
    int64_t key
) {
    if(tree==NULL)return false;

    bool removed=false;

    tree->root=remove_rec(
        tree->root,key,&removed
    );

    if(removed)--tree->size;
    return removed;
}

bool int_order_stat_tree_select(
    const IntOrderStatTree *tree,
    size_t rank,
    int64_t *out_key
) {
    if(tree==NULL||out_key==NULL||rank>=tree->size) {
        return false;
    }

    const OrderNode *node=tree->root;

    while(node!=NULL) {
        const size_t left_size=size_of(node->left);

        if(rank<left_size) {
            node=node->left;
        } else if(rank==left_size) {
            *out_key=node->key;
            return true;
        } else {
            rank-=left_size+1;
            node=node->right;
        }
    }

    return false;
}

bool int_order_stat_tree_rank(
    const IntOrderStatTree *tree,
    int64_t key,
    size_t *out_rank
) {
    if(tree==NULL||out_rank==NULL)return false;

    size_t rank=0;
    const OrderNode *node=tree->root;

    while(node!=NULL) {
        if(key<=node->key) {
            node=node->left;
        } else {
            rank+=size_of(node->left)+1;
            node=node->right;
        }
    }

    *out_rank=rank;
    return true;
}

bool int_order_stat_tree_count_range(
    const IntOrderStatTree *tree,
    int64_t low,
    int64_t high,
    size_t *out_count
) {
    if(tree==NULL||out_count==NULL||low>=high) {
        return false;
    }

    size_t lo=0;
    size_t hi=0;

    if(!int_order_stat_tree_rank(tree,low,&lo)||
       !int_order_stat_tree_rank(tree,high,&hi)) {
        return false;
    }

    *out_count=hi-lo;
    return true;
}

typedef struct {
    bool ok;
    size_t count;
    int height;
    bool has_key;
    int64_t min_key;
    int64_t max_key;
} ValidateResult;

static ValidateResult validate_rec(
    const OrderNode *node
) {
    if(node==NULL) {
        return (ValidateResult){
            .ok=true,
            .count=0,
            .height=0,
            .has_key=false
        };
    }

    ValidateResult left=validate_rec(node->left);
    ValidateResult right=validate_rec(node->right);

    if(!left.ok||!right.ok) {
        return (ValidateResult){.ok=false};
    }

    if(left.has_key&&left.max_key>=node->key) {
        return (ValidateResult){.ok=false};
    }

    if(right.has_key&&right.min_key<=node->key) {
        return (ValidateResult){.ok=false};
    }

    const size_t expected_size=
        left.count+right.count+1;

    const int expected_height=
        1+max_int(left.height,right.height);

    const int bf=left.height-right.height;

    if(node->subtree_size!=expected_size||
       node->height!=expected_height||
       bf<-1||bf>1) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,
        .count=expected_size,
        .height=expected_height,
        .has_key=true,
        .min_key=left.has_key
            ?left.min_key:node->key,
        .max_key=right.has_key
            ?right.max_key:node->key
    };
}

bool int_order_stat_tree_validate(
    const IntOrderStatTree *tree
) {
    if(tree==NULL)return false;

    ValidateResult result=validate_rec(tree->root);

    return result.ok&&
           result.count==tree->size&&
           (tree->root==NULL||tree->root->subtree_size==tree->size);
}
