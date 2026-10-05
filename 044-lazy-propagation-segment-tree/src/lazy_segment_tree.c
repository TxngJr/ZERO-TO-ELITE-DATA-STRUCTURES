#include "lazy_segment_tree.h"

#include <stdint.h>
#include <stdlib.h>

struct LazySegmentTree {
    size_t count;
    size_t capacity;
    int64_t *sum;
    int64_t *min;
    int64_t *lazy;
};

static int64_t min_i64(int64_t a,int64_t b) {
    return a<b?a:b;
}

static void build(
    LazySegmentTree *tree,
    const int64_t *values,
    size_t node,
    size_t left,
    size_t right
) {
    if(right-left==1) {
        tree->sum[node]=values[left];
        tree->min[node]=values[left];
        return;
    }

    const size_t mid=left+(right-left)/2;
    const size_t lc=node*2;
    const size_t rc=lc+1;

    build(tree,values,lc,left,mid);
    build(tree,values,rc,mid,right);

    tree->sum[node]=tree->sum[lc]+tree->sum[rc];
    tree->min[node]=min_i64(tree->min[lc],tree->min[rc]);
}

LazySegmentTree *lazy_segment_tree_create(
    const int64_t *values,
    size_t count
) {
    if(count>0&&values==NULL)return NULL;
    if(count>(size_t)INT64_MAX)return NULL;

    LazySegmentTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->count=count;

    if(count==0)return tree;

    if(count>(SIZE_MAX-8U)/4U) {
        free(tree);
        return NULL;
    }

    tree->capacity=count*4U+8U;

    if(tree->capacity>SIZE_MAX/sizeof *tree->sum) {
        free(tree);
        return NULL;
    }

    tree->sum=calloc(tree->capacity,sizeof *tree->sum);
    tree->min=calloc(tree->capacity,sizeof *tree->min);
    tree->lazy=calloc(tree->capacity,sizeof *tree->lazy);

    if(tree->sum==NULL||tree->min==NULL||tree->lazy==NULL) {
        lazy_segment_tree_free(tree);
        return NULL;
    }

    build(tree,values,1,0,count);
    return tree;
}

void lazy_segment_tree_free(LazySegmentTree *tree) {
    if(tree==NULL)return;
    free(tree->sum);
    free(tree->min);
    free(tree->lazy);
    free(tree);
}

size_t lazy_segment_tree_size(const LazySegmentTree *tree) {
    return tree==NULL?0:tree->count;
}

static void apply(
    LazySegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    int64_t delta
) {
    const int64_t length=(int64_t)(right-left);

    tree->sum[node]+=delta*length;
    tree->min[node]+=delta;
    tree->lazy[node]+=delta;
}

static void push(
    LazySegmentTree *tree,
    size_t node,
    size_t left,
    size_t right
) {
    const int64_t delta=tree->lazy[node];

    if(delta==0||right-left==1)return;

    const size_t mid=left+(right-left)/2;

    apply(tree,node*2,left,mid,delta);
    apply(tree,node*2+1,mid,right,delta);

    tree->lazy[node]=0;
}

static void range_add_rec(
    LazySegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    size_t ql,
    size_t qr,
    int64_t delta
) {
    if(qr<=left||right<=ql)return;

    if(ql<=left&&right<=qr) {
        apply(tree,node,left,right,delta);
        return;
    }

    push(tree,node,left,right);

    const size_t mid=left+(right-left)/2;
    const size_t lc=node*2;
    const size_t rc=lc+1;

    range_add_rec(tree,lc,left,mid,ql,qr,delta);
    range_add_rec(tree,rc,mid,right,ql,qr,delta);

    tree->sum[node]=tree->sum[lc]+tree->sum[rc];
    tree->min[node]=min_i64(tree->min[lc],tree->min[rc]);
}

bool lazy_segment_tree_range_add(
    LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t delta
) {
    if(tree==NULL||left>=right||right>tree->count) {
        return false;
    }

    range_add_rec(
        tree,1,0,tree->count,left,right,delta
    );
    return true;
}

typedef struct {
    int64_t sum;
    int64_t min;
    bool has_value;
} QueryResult;

static QueryResult query_rec(
    const LazySegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    size_t ql,
    size_t qr,
    int64_t carry
) {
    if(qr<=left||right<=ql) {
        return (QueryResult){0};
    }

    if(ql<=left&&right<=qr) {
        const int64_t length=(int64_t)(right-left);

        return (QueryResult){
            .sum=tree->sum[node]+carry*length,
            .min=tree->min[node]+carry,
            .has_value=true
        };
    }

    const int64_t child_carry=carry+tree->lazy[node];
    const size_t mid=left+(right-left)/2;

    QueryResult a=query_rec(
        tree,node*2,left,mid,ql,qr,child_carry
    );
    QueryResult b=query_rec(
        tree,node*2+1,mid,right,ql,qr,child_carry
    );

    if(!a.has_value)return b;
    if(!b.has_value)return a;

    return (QueryResult){
        .sum=a.sum+b.sum,
        .min=min_i64(a.min,b.min),
        .has_value=true
    };
}

bool lazy_segment_tree_range_sum(
    const LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
) {
    if(tree==NULL||out_sum==NULL||
       left>=right||right>tree->count) {
        return false;
    }

    QueryResult result=query_rec(
        tree,1,0,tree->count,left,right,0
    );

    if(!result.has_value)return false;

    *out_sum=result.sum;
    return true;
}

bool lazy_segment_tree_range_min(
    const LazySegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_min
) {
    if(tree==NULL||out_min==NULL||
       left>=right||right>tree->count) {
        return false;
    }

    QueryResult result=query_rec(
        tree,1,0,tree->count,left,right,0
    );

    if(!result.has_value)return false;

    *out_min=result.min;
    return true;
}

bool lazy_segment_tree_point_get(
    const LazySegmentTree *tree,
    size_t index,
    int64_t *out_value
) {
    if(tree==NULL||out_value==NULL||index>=tree->count) {
        return false;
    }

    return lazy_segment_tree_range_sum(
        tree,index,index+1,out_value
    );
}

static bool validate_rec(
    const LazySegmentTree *tree,
    size_t node,
    size_t left,
    size_t right
) {
    if(node>=tree->capacity)return false;

    if(right-left==1) {
        return tree->sum[node]==tree->min[node];
    }

    const size_t mid=left+(right-left)/2;
    const size_t lc=node*2;
    const size_t rc=lc+1;

    if(!validate_rec(tree,lc,left,mid)||
       !validate_rec(tree,rc,mid,right)) {
        return false;
    }

    const int64_t length=(int64_t)(right-left);
    const int64_t expected_sum=
        tree->sum[lc]+tree->sum[rc]+
        tree->lazy[node]*length;
    const int64_t expected_min=
        min_i64(tree->min[lc],tree->min[rc])+
        tree->lazy[node];

    return tree->sum[node]==expected_sum &&
           tree->min[node]==expected_min;
}

bool lazy_segment_tree_validate(
    const LazySegmentTree *tree
) {
    if(tree==NULL)return false;

    if(tree->count==0) {
        return tree->capacity==0 &&
               tree->sum==NULL &&
               tree->min==NULL &&
               tree->lazy==NULL;
    }

    if(tree->sum==NULL||tree->min==NULL||
       tree->lazy==NULL||tree->capacity==0) {
        return false;
    }

    return validate_rec(tree,1,0,tree->count);
}
