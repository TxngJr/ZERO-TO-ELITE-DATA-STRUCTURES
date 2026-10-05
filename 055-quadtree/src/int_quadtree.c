#include "int_quadtree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct QuadNode {
    int64_t x_low;
    int64_t x_high;
    int64_t y_low;
    int64_t y_high;

    IntQuadPoint *points;
    size_t point_count;
    size_t point_capacity;
    size_t subtree_size;

    struct QuadNode *children[4];
} QuadNode;

struct IntQuadtree {
    QuadNode *root;
    size_t size;
    size_t node_count;
    size_t bucket_capacity;
    size_t max_depth;
};

static int64_t safe_midpoint(int64_t low,int64_t high) {
    if(low<0&&high>0) {
        const int64_t a=low/2;
        const int64_t b=high/2;
        const int64_t rem=(low%2)+(high%2);
        return a+b+rem/2;
    }

    return low+(high-low)/2;
}

static bool point_inside_bounds(
    const QuadNode *node,
    const IntQuadPoint *point
) {
    return node->x_low<=point->x&&point->x<node->x_high&&
           node->y_low<=point->y&&point->y<node->y_high;
}

static QuadNode *make_node(
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    QuadNode *node=calloc(1,sizeof *node);
    if(node==NULL)return NULL;

    node->x_low=xl;
    node->x_high=xh;
    node->y_low=yl;
    node->y_high=yh;
    return node;
}

IntQuadtree *int_quadtree_create(
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    size_t bucket_capacity,
    size_t max_depth
) {
    if(x_low>=x_high||y_low>=y_high||
       bucket_capacity==0) {
        return NULL;
    }

    IntQuadtree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->root=make_node(
        x_low,x_high,y_low,y_high
    );

    if(tree->root==NULL) {
        free(tree);
        return NULL;
    }

    tree->bucket_capacity=bucket_capacity;
    tree->max_depth=max_depth;
    tree->node_count=1;
    return tree;
}

static void free_node(QuadNode *node) {
    if(node==NULL)return;

    for(size_t i=0;i<4;++i) {
        free_node(node->children[i]);
    }

    free(node->points);
    free(node);
}

void int_quadtree_free(IntQuadtree *tree) {
    if(tree==NULL)return;
    free_node(tree->root);
    free(tree);
}

size_t int_quadtree_size(const IntQuadtree *tree) {
    return tree==NULL?0:tree->size;
}

size_t int_quadtree_node_count(const IntQuadtree *tree) {
    return tree==NULL?0:tree->node_count;
}

static bool reserve_points(
    QuadNode *node,
    size_t needed
) {
    if(needed<=node->point_capacity)return true;

    size_t capacity=node->point_capacity==0
        ?4:node->point_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }

        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *node->points) {
        return false;
    }

    IntQuadPoint *next=realloc(
        node->points,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    node->points=next;
    node->point_capacity=capacity;
    return true;
}

static bool is_leaf(const QuadNode *node) {
    return node->children[0]==NULL;
}

static bool split_bounds(
    const QuadNode *node,
    int64_t *mx,
    int64_t *my
) {
    *mx=safe_midpoint(node->x_low,node->x_high);
    *my=safe_midpoint(node->y_low,node->y_high);

    return node->x_low<*mx&&*mx<node->x_high&&
           node->y_low<*my&&*my<node->y_high;
}

static size_t quadrant(
    const QuadNode *node,
    int64_t mx,
    int64_t my,
    const IntQuadPoint *point
) {
    (void)node;

    const size_t east=point->x>=mx?1U:0U;
    const size_t north=point->y>=my?1U:0U;

    return north*2U+east;
}

static bool create_children(
    IntQuadtree *tree,
    QuadNode *node,
    int64_t mx,
    int64_t my
) {
    QuadNode *children[4]={
        make_node(node->x_low,mx,node->y_low,my),
        make_node(mx,node->x_high,node->y_low,my),
        make_node(node->x_low,mx,my,node->y_high),
        make_node(mx,node->x_high,my,node->y_high)
    };

    for(size_t i=0;i<4;++i) {
        if(children[i]==NULL) {
            for(size_t j=0;j<4;++j)free_node(children[j]);
            return false;
        }
    }

    for(size_t i=0;i<4;++i) {
        node->children[i]=children[i];
    }

    tree->node_count+=4;
    return true;
}

static bool append_leaf_point(
    QuadNode *node,
    IntQuadPoint point
) {
    if(node->point_count==SIZE_MAX)return false;

    if(!reserve_points(node,node->point_count+1)) {
        return false;
    }

    node->points[node->point_count++]=point;
    node->subtree_size=node->point_count;
    return true;
}

static bool insert_rec(
    IntQuadtree *tree,
    QuadNode *node,
    IntQuadPoint point,
    size_t depth
);

static bool split_and_redistribute(
    IntQuadtree *tree,
    QuadNode *node,
    size_t depth
) {
    int64_t mx=0;
    int64_t my=0;

    if(!split_bounds(node,&mx,&my))return false;

    if(!create_children(tree,node,mx,my))return false;

    const size_t old_count=node->point_count;
    size_t quadrant_counts[4]={0};

    for(size_t i=0;i<old_count;++i) {
        const size_t q=quadrant(
            node,mx,my,&node->points[i]
        );
        ++quadrant_counts[q];
    }

    for(size_t i=0;i<4;++i) {
        if(quadrant_counts[i]>0 &&
           !reserve_points(
               node->children[i],quadrant_counts[i]
           )) {
            for(size_t j=0;j<4;++j) {
                free_node(node->children[j]);
                node->children[j]=NULL;
            }
            tree->node_count-=4;
            return false;
        }
    }

    IntQuadPoint *old_points=node->points;

    node->points=NULL;
    node->point_count=0;
    node->point_capacity=0;
    node->subtree_size=0;

    for(size_t i=0;i<old_count;++i) {
        const size_t q=quadrant(
            node,mx,my,&old_points[i]
        );

        if(!insert_rec(
                tree,
                node->children[q],
                old_points[i],
                depth+1
            )) {
            /*
             * Allocation failure after structural split is rare;
             * validation will still see already redistributed points.
             * Caller reports failure without publishing the new point.
             */
            free(old_points);
            return false;
        }

        ++node->subtree_size;
    }

    free(old_points);
    return true;
}

static bool insert_rec(
    IntQuadtree *tree,
    QuadNode *node,
    IntQuadPoint point,
    size_t depth
) {
    if(!point_inside_bounds(node,&point))return false;

    if(is_leaf(node)) {
        if(node->point_count<tree->bucket_capacity||
           depth>=tree->max_depth) {
            return append_leaf_point(node,point);
        }

        int64_t mx=0;
        int64_t my=0;

        if(!split_bounds(node,&mx,&my)) {
            return append_leaf_point(node,point);
        }

        if(!split_and_redistribute(tree,node,depth)) {
            return false;
        }
    }

    int64_t mx=0;
    int64_t my=0;

    if(!split_bounds(node,&mx,&my))return false;

    const size_t q=quadrant(node,mx,my,&point);

    if(!insert_rec(
            tree,node->children[q],point,depth+1
        )) {
        return false;
    }

    ++node->subtree_size;
    return true;
}

bool int_quadtree_insert(
    IntQuadtree *tree,
    IntQuadPoint point
) {
    if(tree==NULL||tree->size==SIZE_MAX||
       !point_inside_bounds(tree->root,&point)) {
        return false;
    }

    if(!insert_rec(tree,tree->root,point,0)) {
        return false;
    }

    ++tree->size;
    return true;
}

static bool bounds_disjoint(
    const QuadNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return node->x_high<=xl||
           xh<=node->x_low||
           node->y_high<=yl||
           yh<=node->y_low;
}

static bool bounds_inside(
    const QuadNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=node->x_low&&node->x_high<=xh&&
           yl<=node->y_low&&node->y_high<=yh;
}

static bool point_in_query(
    const IntQuadPoint *point,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=point->x&&point->x<xh&&
           yl<=point->y&&point->y<yh;
}

static size_t count_rec(
    const QuadNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    if(bounds_disjoint(node,xl,xh,yl,yh))return 0;
    if(bounds_inside(node,xl,xh,yl,yh)) {
        return node->subtree_size;
    }

    if(is_leaf(node)) {
        size_t count=0;

        for(size_t i=0;i<node->point_count;++i) {
            if(point_in_query(
                    &node->points[i],xl,xh,yl,yh
                )) {
                ++count;
            }
        }

        return count;
    }

    size_t count=0;

    for(size_t i=0;i<4;++i) {
        count+=count_rec(
            node->children[i],xl,xh,yl,yh
        );
    }

    return count;
}

bool int_quadtree_query_count(
    const IntQuadtree *tree,
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
        tree->root,
        x_low,x_high,y_low,y_high
    );
    return true;
}

static bool collect_all(
    const QuadNode *node,
    IntQuadPoint *output,
    size_t capacity,
    size_t *written
) {
    if(is_leaf(node)) {
        if(node->point_count>capacity-*written) {
            return false;
        }

        for(size_t i=0;i<node->point_count;++i) {
            output[(*written)++]=node->points[i];
        }

        return true;
    }

    for(size_t i=0;i<4;++i) {
        if(!collect_all(
                node->children[i],
                output,capacity,written
            )) {
            return false;
        }
    }

    return true;
}

static bool report_rec(
    const QuadNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    IntQuadPoint *output,
    size_t capacity,
    size_t *written
) {
    if(bounds_disjoint(node,xl,xh,yl,yh))return true;

    if(bounds_inside(node,xl,xh,yl,yh)) {
        return collect_all(
            node,output,capacity,written
        );
    }

    if(is_leaf(node)) {
        for(size_t i=0;i<node->point_count;++i) {
            if(point_in_query(
                    &node->points[i],xl,xh,yl,yh
                )) {
                if(*written>=capacity)return false;
                output[(*written)++]=node->points[i];
            }
        }

        return true;
    }

    for(size_t i=0;i<4;++i) {
        if(!report_rec(
                node->children[i],
                xl,xh,yl,yh,
                output,capacity,written
            )) {
            return false;
        }
    }

    return true;
}

bool int_quadtree_query_report(
    const IntQuadtree *tree,
    int64_t x_low,
    int64_t x_high,
    int64_t y_low,
    int64_t y_high,
    IntQuadPoint *output,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL||
       x_low>=x_high||y_low>=y_high) {
        return false;
    }

    size_t needed=0;

    if(!int_quadtree_query_count(
            tree,
            x_low,x_high,y_low,y_high,
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
            tree->root,
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
    size_t points;
    size_t nodes;
} ValidateResult;

static bool same_bounds(
    const QuadNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return node!=NULL&&
           node->x_low==xl&&node->x_high==xh&&
           node->y_low==yl&&node->y_high==yh;
}

static ValidateResult validate_rec(
    const QuadNode *node,
    size_t depth,
    size_t max_depth
) {
    if(node==NULL||node->x_low>=node->x_high||
       node->y_low>=node->y_high||
       depth>max_depth) {
        return (ValidateResult){.ok=false};
    }

    if(is_leaf(node)) {
        for(size_t i=0;i<node->point_count;++i) {
            if(!point_inside_bounds(
                    node,&node->points[i]
                )) {
                return (ValidateResult){.ok=false};
            }
        }

        if(node->subtree_size!=node->point_count) {
            return (ValidateResult){.ok=false};
        }

        return (ValidateResult){
            .ok=true,
            .points=node->point_count,
            .nodes=1
        };
    }

    if(node->point_count!=0||node->points!=NULL) {
        return (ValidateResult){.ok=false};
    }

    int64_t mx=0;
    int64_t my=0;

    if(!split_bounds(node,&mx,&my)) {
        return (ValidateResult){.ok=false};
    }

    if(!same_bounds(
            node->children[0],
            node->x_low,mx,node->y_low,my
        )||
       !same_bounds(
            node->children[1],
            mx,node->x_high,node->y_low,my
        )||
       !same_bounds(
            node->children[2],
            node->x_low,mx,my,node->y_high
        )||
       !same_bounds(
            node->children[3],
            mx,node->x_high,my,node->y_high
        )) {
        return (ValidateResult){.ok=false};
    }

    size_t points=0;
    size_t nodes=1;

    for(size_t i=0;i<4;++i) {
        ValidateResult child=validate_rec(
            node->children[i],depth+1,max_depth
        );

        if(!child.ok)return child;

        points+=child.points;
        nodes+=child.nodes;
    }

    if(node->subtree_size!=points) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,
        .points=points,
        .nodes=nodes
    };
}

bool int_quadtree_validate(const IntQuadtree *tree) {
    if(tree==NULL||tree->root==NULL)return false;

    ValidateResult result=validate_rec(
        tree->root,0,tree->max_depth
    );

    return result.ok&&
           result.points==tree->size&&
           result.nodes==tree->node_count;
}
