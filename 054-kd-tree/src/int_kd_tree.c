#include "int_kd_tree.h"

#include <float.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    IntKDPoint point;
    size_t serial;
} BuildEntry;

typedef struct {
    IntKDPoint point;
    unsigned axis;
    size_t left;
    size_t right;
    size_t subtree_size;
    int64_t min_x;
    int64_t max_x;
    int64_t min_y;
    int64_t max_y;
} KDNode;

struct IntKDTree {
    KDNode *nodes;
    size_t count;
    size_t root;
};

static int compare_common(
    const BuildEntry *a,
    const BuildEntry *b,
    unsigned axis
) {
    if(axis==0U) {
        if(a->point.x<b->point.x)return -1;
        if(a->point.x>b->point.x)return 1;
        if(a->point.y<b->point.y)return -1;
        if(a->point.y>b->point.y)return 1;
    } else {
        if(a->point.y<b->point.y)return -1;
        if(a->point.y>b->point.y)return 1;
        if(a->point.x<b->point.x)return -1;
        if(a->point.x>b->point.x)return 1;
    }

    if(a->point.id<b->point.id)return -1;
    if(a->point.id>b->point.id)return 1;
    if(a->serial<b->serial)return -1;
    if(a->serial>b->serial)return 1;
    return 0;
}

static int compare_x(const void *a,const void *b) {
    return compare_common(a,b,0U);
}

static int compare_y(const void *a,const void *b) {
    return compare_common(a,b,1U);
}

static int64_t min_i64(int64_t a,int64_t b) {
    return a<b?a:b;
}

static int64_t max_i64(int64_t a,int64_t b) {
    return a>b?a:b;
}

static size_t build_rec(
    IntKDTree *tree,
    BuildEntry *entries,
    size_t begin,
    size_t end,
    size_t depth,
    size_t *next
) {
    if(begin>=end)return SIZE_MAX;

    const unsigned axis=(unsigned)(depth&1U);

    qsort(
        entries+begin,
        end-begin,
        sizeof *entries,
        axis==0U?compare_x:compare_y
    );

    const size_t mid=begin+(end-begin)/2;
    const size_t node_index=(*next)++;

    KDNode *node=&tree->nodes[node_index];
    node->point=entries[mid].point;
    node->axis=axis;

    node->left=build_rec(
        tree,entries,begin,mid,depth+1,next
    );
    node->right=build_rec(
        tree,entries,mid+1,end,depth+1,next
    );

    node->subtree_size=1;
    node->min_x=node->max_x=node->point.x;
    node->min_y=node->max_y=node->point.y;

    if(node->left!=SIZE_MAX) {
        const KDNode *left=&tree->nodes[node->left];

        node->subtree_size+=left->subtree_size;
        node->min_x=min_i64(node->min_x,left->min_x);
        node->max_x=max_i64(node->max_x,left->max_x);
        node->min_y=min_i64(node->min_y,left->min_y);
        node->max_y=max_i64(node->max_y,left->max_y);
    }

    if(node->right!=SIZE_MAX) {
        const KDNode *right=&tree->nodes[node->right];

        node->subtree_size+=right->subtree_size;
        node->min_x=min_i64(node->min_x,right->min_x);
        node->max_x=max_i64(node->max_x,right->max_x);
        node->min_y=min_i64(node->min_y,right->min_y);
        node->max_y=max_i64(node->max_y,right->max_y);
    }

    return node_index;
}

IntKDTree *int_kd_tree_create(
    const IntKDPoint *points,
    size_t count
) {
    if(count>0&&points==NULL)return NULL;

    IntKDTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->count=count;
    tree->root=SIZE_MAX;

    if(count==0)return tree;

    if(count>SIZE_MAX/sizeof *tree->nodes ||
       count>SIZE_MAX/sizeof(BuildEntry)) {
        free(tree);
        return NULL;
    }

    tree->nodes=calloc(count,sizeof *tree->nodes);
    BuildEntry *entries=malloc(count*sizeof *entries);

    if(tree->nodes==NULL||entries==NULL) {
        free(entries);
        int_kd_tree_free(tree);
        return NULL;
    }

    for(size_t i=0;i<count;++i) {
        entries[i]=(BuildEntry){
            .point=points[i],
            .serial=i
        };
    }

    size_t next=0;

    tree->root=build_rec(
        tree,entries,0,count,0,&next
    );

    free(entries);

    if(next!=count) {
        int_kd_tree_free(tree);
        return NULL;
    }

    return tree;
}

void int_kd_tree_free(IntKDTree *tree) {
    if(tree==NULL)return;
    free(tree->nodes);
    free(tree);
}

size_t int_kd_tree_size(const IntKDTree *tree) {
    return tree==NULL?0:tree->count;
}

static bool box_disjoint(
    const KDNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return node->max_x<xl||
           node->min_x>=xh||
           node->max_y<yl||
           node->min_y>=yh;
}

static bool box_inside(
    const KDNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=node->min_x&&node->max_x<xh&&
           yl<=node->min_y&&node->max_y<yh;
}

static bool point_inside(
    const IntKDPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh;
}

static size_t count_rec(
    const IntKDTree *tree,
    size_t index,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    if(index==SIZE_MAX)return 0;

    const KDNode *node=&tree->nodes[index];

    if(box_disjoint(node,xl,xh,yl,yh))return 0;
    if(box_inside(node,xl,xh,yl,yh)) {
        return node->subtree_size;
    }

    size_t count=point_inside(
        &node->point,xl,xh,yl,yh
    )?1U:0U;

    count+=count_rec(
        tree,node->left,xl,xh,yl,yh
    );
    count+=count_rec(
        tree,node->right,xl,xh,yl,yh
    );

    return count;
}

bool int_kd_tree_query_count(
    const IntKDTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t *out_count
) {
    if(tree==NULL||out_count==NULL||
       x_low>=x_high||y_low>=y_high) {
        return false;
    }

    *out_count=count_rec(
        tree,tree->root,
        x_low,x_high,y_low,y_high
    );
    return true;
}

static bool collect_subtree(
    const IntKDTree *tree,
    size_t index,
    IntKDPoint *output,
    size_t capacity,
    size_t *written
) {
    if(index==SIZE_MAX)return true;

    if(*written>=capacity)return false;

    const KDNode *node=&tree->nodes[index];
    output[(*written)++]=node->point;

    return collect_subtree(
               tree,node->left,
               output,capacity,written
           )&&
           collect_subtree(
               tree,node->right,
               output,capacity,written
           );
}

static bool report_rec(
    const IntKDTree *tree,
    size_t index,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    IntKDPoint *output,
    size_t capacity,
    size_t *written
) {
    if(index==SIZE_MAX)return true;

    const KDNode *node=&tree->nodes[index];

    if(box_disjoint(node,xl,xh,yl,yh))return true;

    if(box_inside(node,xl,xh,yl,yh)) {
        return collect_subtree(
            tree,index,output,capacity,written
        );
    }

    if(point_inside(&node->point,xl,xh,yl,yh)) {
        if(*written>=capacity)return false;
        output[(*written)++]=node->point;
    }

    return report_rec(
               tree,node->left,
               xl,xh,yl,yh,
               output,capacity,written
           )&&
           report_rec(
               tree,node->right,
               xl,xh,yl,yh,
               output,capacity,written
           );
}

bool int_kd_tree_query_report(
    const IntKDTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntKDPoint *output,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL||
       x_low>=x_high||y_low>=y_high) {
        return false;
    }

    size_t needed=0;

    if(!int_kd_tree_query_count(
            tree,x_low,x_high,y_low,y_high,&needed
        )) {
        return false;
    }

    if(needed>capacity||
       (needed>0&&output==NULL)) {
        return false;
    }

    size_t written=0;

    if(!report_rec(
            tree,tree->root,
            x_low,x_high,y_low,y_high,
            output,capacity,&written
        )) {
        return false;
    }

    *out_written=written;
    return written==needed;
}

static long double point_distance_sq(
    const IntKDPoint *point,
    int64_t qx,
    int64_t qy
) {
    const long double dx=
        (long double)point->x-(long double)qx;
    const long double dy=
        (long double)point->y-(long double)qy;

    return dx*dx+dy*dy;
}

static long double box_distance_sq(
    const KDNode *node,
    int64_t qx,
    int64_t qy
) {
    long double dx=0.0L;
    long double dy=0.0L;

    if(qx<node->min_x) {
        dx=(long double)node->min_x-(long double)qx;
    } else if(qx>node->max_x) {
        dx=(long double)qx-(long double)node->max_x;
    }

    if(qy<node->min_y) {
        dy=(long double)node->min_y-(long double)qy;
    } else if(qy>node->max_y) {
        dy=(long double)qy-(long double)node->max_y;
    }

    return dx*dx+dy*dy;
}

static bool point_lex_less(
    const IntKDPoint *a,
    const IntKDPoint *b
) {
    if(a->x!=b->x)return a->x<b->x;
    if(a->y!=b->y)return a->y<b->y;
    return a->id<b->id;
}

typedef struct {
    bool has;
    IntKDPoint point;
    long double distance;
} NearestState;

static void nearest_rec(
    const IntKDTree *tree,
    size_t index,
    int64_t qx,
    int64_t qy,
    NearestState *best
) {
    if(index==SIZE_MAX)return;

    const KDNode *node=&tree->nodes[index];
    const long double distance=
        point_distance_sq(&node->point,qx,qy);

    if(!best->has||
       distance<best->distance||
       (distance==best->distance&&
        point_lex_less(&node->point,&best->point))) {
        best->has=true;
        best->point=node->point;
        best->distance=distance;
    }

    const size_t a=node->left;
    const size_t b=node->right;

    long double da=LDBL_MAX;
    long double db=LDBL_MAX;

    if(a!=SIZE_MAX) {
        da=box_distance_sq(&tree->nodes[a],qx,qy);
    }

    if(b!=SIZE_MAX) {
        db=box_distance_sq(&tree->nodes[b],qx,qy);
    }

    size_t first=a;
    size_t second=b;
    long double first_d=da;
    long double second_d=db;

    if(db<da) {
        first=b;
        second=a;
        first_d=db;
        second_d=da;
    }

    if(first!=SIZE_MAX&&
       first_d<=best->distance) {
        nearest_rec(tree,first,qx,qy,best);
    }

    if(second!=SIZE_MAX&&
       second_d<=best->distance) {
        nearest_rec(tree,second,qx,qy,best);
    }
}

bool int_kd_tree_nearest(
    const IntKDTree *tree,
    int64_t query_x,
    int64_t query_y,
    IntKDPoint *out_point,
    long double *out_squared_distance
) {
    if(tree==NULL||out_point==NULL||
       out_squared_distance==NULL||
       tree->count==0) {
        return false;
    }

    NearestState best={
        .has=false,
        .distance=LDBL_MAX
    };

    nearest_rec(
        tree,tree->root,
        query_x,query_y,&best
    );

    if(!best.has)return false;

    *out_point=best.point;
    *out_squared_distance=best.distance;
    return true;
}

typedef struct {
    bool ok;
    size_t count;
    int64_t min_x;
    int64_t max_x;
    int64_t min_y;
    int64_t max_y;
    bool has;
} ValidateResult;

static ValidateResult validate_rec(
    const IntKDTree *tree,
    size_t index,
    size_t depth,
    bool *seen
) {
    if(index==SIZE_MAX) {
        return (ValidateResult){
            .ok=true,
            .has=false
        };
    }

    if(index>=tree->count||seen[index]) {
        return (ValidateResult){.ok=false};
    }

    seen[index]=true;

    const KDNode *node=&tree->nodes[index];

    if(node->axis!=(unsigned)(depth&1U)) {
        return (ValidateResult){.ok=false};
    }

    ValidateResult left=validate_rec(
        tree,node->left,depth+1,seen
    );
    ValidateResult right=validate_rec(
        tree,node->right,depth+1,seen
    );

    if(!left.ok||!right.ok) {
        return (ValidateResult){.ok=false};
    }

    if(node->axis==0U) {
        if(left.has&&left.max_x>node->point.x) {
            return (ValidateResult){.ok=false};
        }

        if(right.has&&right.min_x<node->point.x) {
            return (ValidateResult){.ok=false};
        }
    } else {
        if(left.has&&left.max_y>node->point.y) {
            return (ValidateResult){.ok=false};
        }

        if(right.has&&right.min_y<node->point.y) {
            return (ValidateResult){.ok=false};
        }
    }

    size_t count=1;
    int64_t min_x=node->point.x;
    int64_t max_x=node->point.x;
    int64_t min_y=node->point.y;
    int64_t max_y=node->point.y;

    if(left.has) {
        count+=left.count;
        min_x=min_i64(min_x,left.min_x);
        max_x=max_i64(max_x,left.max_x);
        min_y=min_i64(min_y,left.min_y);
        max_y=max_i64(max_y,left.max_y);
    }

    if(right.has) {
        count+=right.count;
        min_x=min_i64(min_x,right.min_x);
        max_x=max_i64(max_x,right.max_x);
        min_y=min_i64(min_y,right.min_y);
        max_y=max_i64(max_y,right.max_y);
    }

    if(node->subtree_size!=count||
       node->min_x!=min_x||
       node->max_x!=max_x||
       node->min_y!=min_y||
       node->max_y!=max_y) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,
        .count=count,
        .min_x=min_x,
        .max_x=max_x,
        .min_y=min_y,
        .max_y=max_y,
        .has=true
    };
}

bool int_kd_tree_validate(const IntKDTree *tree) {
    if(tree==NULL)return false;

    if(tree->count==0) {
        return tree->root==SIZE_MAX&&tree->nodes==NULL;
    }

    if(tree->nodes==NULL||tree->root>=tree->count) {
        return false;
    }

    bool *seen=calloc(tree->count,sizeof *seen);

    if(seen==NULL)return false;

    ValidateResult result=validate_rec(
        tree,tree->root,0,seen
    );

    if(!result.ok||result.count!=tree->count) {
        free(seen);
        return false;
    }

    for(size_t i=0;i<tree->count;++i) {
        if(!seen[i]) {
            free(seen);
            return false;
        }
    }

    free(seen);
    return true;
}
