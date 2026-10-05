#include "int_range_tree.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct RangeNode {
    size_t point_index;
    int64_t min_x;
    int64_t max_x;
    size_t *y_indices;
    size_t y_count;
    struct RangeNode *left;
    struct RangeNode *right;
} RangeNode;

struct IntRangeTree {
    IntPoint2D *points;
    size_t count;
    RangeNode *root;
};

static int point_x_compare_qsort(
    const void *a,
    const void *b
) {
    const IntPoint2D *pa=a;
    const IntPoint2D *pb=b;

    if(pa->x<pb->x)return -1;
    if(pa->x>pb->x)return 1;
    if(pa->y<pb->y)return -1;
    if(pa->y>pb->y)return 1;
    if(pa->id<pb->id)return -1;
    if(pa->id>pb->id)return 1;
    return 0;
}

static bool y_index_less(
    const IntRangeTree *tree,
    size_t a,
    size_t b
) {
    const IntPoint2D *pa=&tree->points[a];
    const IntPoint2D *pb=&tree->points[b];

    if(pa->y!=pb->y)return pa->y<pb->y;
    if(pa->x!=pb->x)return pa->x<pb->x;
    if(pa->id!=pb->id)return pa->id<pb->id;
    return a<b;
}

static void free_node(RangeNode *node) {
    if(node==NULL)return;
    free_node(node->left);
    free_node(node->right);
    free(node->y_indices);
    free(node);
}

static bool merge_y_indices(
    const IntRangeTree *tree,
    RangeNode *node
) {
    const size_t left_count=
        node->left==NULL?0:node->left->y_count;
    const size_t right_count=
        node->right==NULL?0:node->right->y_count;

    if(left_count>SIZE_MAX-right_count-1)return false;

    node->y_count=left_count+right_count+1;

    if(node->y_count>SIZE_MAX/sizeof *node->y_indices) {
        return false;
    }

    node->y_indices=malloc(
        node->y_count*sizeof *node->y_indices
    );
    if(node->y_indices==NULL)return false;

    size_t li=0;
    size_t ri=0;
    size_t out=0;

    while(li<left_count||ri<right_count) {
        bool take_left=false;

        if(ri>=right_count) {
            take_left=true;
        } else if(li<left_count) {
            take_left=y_index_less(
                tree,
                node->left->y_indices[li],
                node->right->y_indices[ri]
            );
        }

        if(take_left) {
            node->y_indices[out++]=
                node->left->y_indices[li++];
        } else {
            node->y_indices[out++]=
                node->right->y_indices[ri++];
        }
    }

    const size_t pivot=node->point_index;
    size_t pos=out;

    while(pos>0 &&
          y_index_less(
              tree,pivot,node->y_indices[pos-1]
          )) {
        node->y_indices[pos]=node->y_indices[pos-1];
        --pos;
    }

    node->y_indices[pos]=pivot;
    return true;
}

static RangeNode *build_rec(
    IntRangeTree *tree,
    size_t begin,
    size_t end,
    bool *ok
) {
    if(begin>=end)return NULL;

    const size_t mid=begin+(end-begin)/2;

    RangeNode *node=calloc(1,sizeof *node);
    if(node==NULL) {
        *ok=false;
        return NULL;
    }

    node->point_index=mid;

    node->left=build_rec(tree,begin,mid,ok);

    if(!*ok) {
        free_node(node);
        return NULL;
    }

    node->right=build_rec(tree,mid+1,end,ok);

    if(!*ok) {
        free_node(node);
        return NULL;
    }

    node->min_x=node->left!=NULL
        ?node->left->min_x
        :tree->points[mid].x;

    node->max_x=node->right!=NULL
        ?node->right->max_x
        :tree->points[mid].x;

    if(!merge_y_indices(tree,node)) {
        *ok=false;
        free_node(node);
        return NULL;
    }

    return node;
}

IntRangeTree *int_range_tree_create(
    const IntPoint2D *points,
    size_t count
) {
    if(count>0&&points==NULL)return NULL;

    IntRangeTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->count=count;

    if(count==0)return tree;

    if(count>SIZE_MAX/sizeof *tree->points) {
        free(tree);
        return NULL;
    }

    tree->points=malloc(count*sizeof *tree->points);
    if(tree->points==NULL) {
        free(tree);
        return NULL;
    }

    memcpy(
        tree->points,
        points,
        count*sizeof *tree->points
    );

    qsort(
        tree->points,
        count,
        sizeof *tree->points,
        point_x_compare_qsort
    );

    bool ok=true;
    tree->root=build_rec(tree,0,count,&ok);

    if(!ok) {
        int_range_tree_free(tree);
        return NULL;
    }

    return tree;
}

void int_range_tree_free(IntRangeTree *tree) {
    if(tree==NULL)return;
    free_node(tree->root);
    free(tree->points);
    free(tree);
}

size_t int_range_tree_size(const IntRangeTree *tree) {
    return tree==NULL?0:tree->count;
}

static size_t lower_bound_y(
    const IntRangeTree *tree,
    const RangeNode *node,
    int64_t y
) {
    size_t low=0;
    size_t high=node->y_count;

    while(low<high) {
        const size_t mid=low+(high-low)/2;
        const int64_t value=
            tree->points[node->y_indices[mid]].y;

        if(value<y)low=mid+1;
        else high=mid;
    }

    return low;
}

static bool point_in_rect(
    const IntPoint2D *point,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high
) {
    return x_low<=point->x&&point->x<x_high&&
           y_low<=point->y&&point->y<y_high;
}

static size_t count_rec(
    const IntRangeTree *tree,
    const RangeNode *node,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high
) {
    if(node==NULL||
       node->max_x<x_low||
       node->min_x>=x_high) {
        return 0;
    }

    if(x_low<=node->min_x&&node->max_x<x_high) {
        const size_t first=lower_bound_y(
            tree,node,y_low
        );
        const size_t last=lower_bound_y(
            tree,node,y_high
        );
        return last-first;
    }

    size_t count=0;
    const IntPoint2D *point=
        &tree->points[node->point_index];

    if(point_in_rect(
            point,
            x_low,x_high,
            y_low,y_high
        )) {
        ++count;
    }

    count+=count_rec(
        tree,node->left,
        x_low,x_high,y_low,y_high
    );
    count+=count_rec(
        tree,node->right,
        x_low,x_high,y_low,y_high
    );

    return count;
}

bool int_range_tree_query_count(
    const IntRangeTree *tree,
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

static bool report_rec(
    const IntRangeTree *tree,
    const RangeNode *node,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntPoint2D *output,
    size_t capacity,
    size_t *written
) {
    if(node==NULL||
       node->max_x<x_low||
       node->min_x>=x_high) {
        return true;
    }

    if(x_low<=node->min_x&&node->max_x<x_high) {
        const size_t first=lower_bound_y(
            tree,node,y_low
        );
        const size_t last=lower_bound_y(
            tree,node,y_high
        );

        for(size_t i=first;i<last;++i) {
            if(*written>=capacity)return false;

            output[(*written)++]=
                tree->points[node->y_indices[i]];
        }

        return true;
    }

    const IntPoint2D *point=
        &tree->points[node->point_index];

    if(point_in_rect(
            point,
            x_low,x_high,
            y_low,y_high
        )) {
        if(*written>=capacity)return false;
        output[(*written)++]=*point;
    }

    return report_rec(
               tree,node->left,
               x_low,x_high,y_low,y_high,
               output,capacity,written
           )&&
           report_rec(
               tree,node->right,
               x_low,x_high,y_low,y_high,
               output,capacity,written
           );
}

bool int_range_tree_query_report(
    const IntRangeTree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntPoint2D *output,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL||
       x_low>=x_high||y_low>=y_high) {
        return false;
    }

    size_t needed=0;

    if(!int_range_tree_query_count(
            tree,
            x_low,x_high,
            y_low,y_high,
            &needed
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

typedef struct {
    bool ok;
    size_t count;
    int height;
    int64_t min_x;
    int64_t max_x;
    bool has_point;
} ValidateResult;

static bool validate_y_merge(
    const IntRangeTree *tree,
    const RangeNode *node
) {
    const size_t left_count=
        node->left==NULL?0:node->left->y_count;
    const size_t right_count=
        node->right==NULL?0:node->right->y_count;

    size_t li=0;
    size_t ri=0;
    bool pivot_used=false;

    for(size_t out=0;out<node->y_count;++out) {
        size_t candidate=SIZE_MAX;

        if(li<left_count) {
            candidate=node->left->y_indices[li];
        }

        if(ri<right_count) {
            const size_t r=node->right->y_indices[ri];

            if(candidate==SIZE_MAX||
               y_index_less(tree,r,candidate)) {
                candidate=r;
            }
        }

        if(!pivot_used) {
            const size_t p=node->point_index;

            if(candidate==SIZE_MAX||
               y_index_less(tree,p,candidate)) {
                candidate=p;
            }
        }

        if(candidate==SIZE_MAX||
           node->y_indices[out]!=candidate) {
            return false;
        }

        if(!pivot_used&&candidate==node->point_index) {
            pivot_used=true;
        } else if(li<left_count&&
                  candidate==node->left->y_indices[li]) {
            ++li;
        } else if(ri<right_count&&
                  candidate==node->right->y_indices[ri]) {
            ++ri;
        } else {
            return false;
        }
    }

    return pivot_used&&
           li==left_count&&
           ri==right_count;
}

static ValidateResult validate_rec(
    const IntRangeTree *tree,
    const RangeNode *node
) {
    if(node==NULL) {
        return (ValidateResult){
            .ok=true,
            .height=0,
            .has_point=false
        };
    }

    if(node->point_index>=tree->count||
       node->y_indices==NULL||
       node->y_count==0) {
        return (ValidateResult){.ok=false};
    }

    ValidateResult left=validate_rec(tree,node->left);
    ValidateResult right=validate_rec(tree,node->right);

    if(!left.ok||!right.ok) {
        return (ValidateResult){.ok=false};
    }

    const IntPoint2D *point=
        &tree->points[node->point_index];

    if(left.has_point&&left.max_x>point->x) {
        return (ValidateResult){.ok=false};
    }

    if(right.has_point&&right.min_x<point->x) {
        return (ValidateResult){.ok=false};
    }

    const size_t expected_count=
        left.count+right.count+1;

    if(node->y_count!=expected_count||
       !validate_y_merge(tree,node)) {
        return (ValidateResult){.ok=false};
    }

    for(size_t i=1;i<node->y_count;++i) {
        if(y_index_less(
                tree,
                node->y_indices[i],
                node->y_indices[i-1]
            )) {
            return (ValidateResult){.ok=false};
        }
    }

    const int64_t expected_min=
        left.has_point?left.min_x:point->x;
    const int64_t expected_max=
        right.has_point?right.max_x:point->x;

    if(node->min_x!=expected_min||
       node->max_x!=expected_max) {
        return (ValidateResult){.ok=false};
    }

    const int height=
        1+(left.height>right.height
              ?left.height:right.height);

    const int diff=left.height-right.height;

    if(diff<-1||diff>1) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,
        .count=expected_count,
        .height=height,
        .min_x=expected_min,
        .max_x=expected_max,
        .has_point=true
    };
}

bool int_range_tree_validate(
    const IntRangeTree *tree
) {
    if(tree==NULL)return false;

    if(tree->count==0) {
        return tree->points==NULL&&tree->root==NULL;
    }

    if(tree->points==NULL||tree->root==NULL)return false;

    const ValidateResult result=
        validate_rec(tree,tree->root);

    return result.ok&&result.count==tree->count;
}
