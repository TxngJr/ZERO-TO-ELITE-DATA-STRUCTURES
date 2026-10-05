#include "int_octree.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct OctNode {
    int64_t xl,xh,yl,yh,zl,zh;
    IntOctPoint *points;
    size_t point_count;
    size_t point_capacity;
    size_t subtree_size;
    struct OctNode *children[8];
} OctNode;

struct IntOctree {
    OctNode *root;
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

static OctNode *make_node(
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    OctNode *node=calloc(1,sizeof *node);
    if(node==NULL)return NULL;

    node->xl=xl;node->xh=xh;
    node->yl=yl;node->yh=yh;
    node->zl=zl;node->zh=zh;
    return node;
}

static bool point_inside(
    const OctNode *node,
    const IntOctPoint *point
) {
    return node->xl<=point->x&&point->x<node->xh&&
           node->yl<=point->y&&point->y<node->yh&&
           node->zl<=point->z&&point->z<node->zh;
}

IntOctree *int_octree_create(
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh,
    size_t bucket_capacity,
    size_t max_depth
) {
    if(xl>=xh||yl>=yh||zl>=zh||bucket_capacity==0) {
        return NULL;
    }

    IntOctree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->root=make_node(xl,xh,yl,yh,zl,zh);

    if(tree->root==NULL) {
        free(tree);
        return NULL;
    }

    tree->bucket_capacity=bucket_capacity;
    tree->max_depth=max_depth;
    tree->node_count=1;
    return tree;
}

static void free_node(OctNode *node) {
    if(node==NULL)return;

    for(size_t i=0;i<8;++i) {
        free_node(node->children[i]);
    }

    free(node->points);
    free(node);
}

void int_octree_free(IntOctree *tree) {
    if(tree==NULL)return;
    free_node(tree->root);
    free(tree);
}

size_t int_octree_size(const IntOctree *tree) {
    return tree==NULL?0:tree->size;
}

size_t int_octree_node_count(const IntOctree *tree) {
    return tree==NULL?0:tree->node_count;
}

static bool is_leaf(const OctNode *node) {
    return node->children[0]==NULL;
}

static bool reserve_points(OctNode *node,size_t needed) {
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

    IntOctPoint *next=realloc(
        node->points,capacity*sizeof *next
    );

    if(next==NULL)return false;

    node->points=next;
    node->point_capacity=capacity;
    return true;
}

static bool split_mids(
    const OctNode *node,
    int64_t *mx,int64_t *my,int64_t *mz
) {
    *mx=safe_midpoint(node->xl,node->xh);
    *my=safe_midpoint(node->yl,node->yh);
    *mz=safe_midpoint(node->zl,node->zh);

    return node->xl<*mx&&*mx<node->xh&&
           node->yl<*my&&*my<node->yh&&
           node->zl<*mz&&*mz<node->zh;
}

static size_t octant(
    int64_t mx,int64_t my,int64_t mz,
    const IntOctPoint *point
) {
    const size_t ex=point->x>=mx?1U:0U;
    const size_t ny=point->y>=my?1U:0U;
    const size_t uz=point->z>=mz?1U:0U;

    return ex+2U*ny+4U*uz;
}

static bool create_children(
    IntOctree *tree,
    OctNode *node,
    int64_t mx,int64_t my,int64_t mz
) {
    OctNode *children[8]={0};

    for(size_t i=0;i<8;++i) {
        const bool east=(i&1U)!=0U;
        const bool north=(i&2U)!=0U;
        const bool upper=(i&4U)!=0U;

        children[i]=make_node(
            east?mx:node->xl,
            east?node->xh:mx,
            north?my:node->yl,
            north?node->yh:my,
            upper?mz:node->zl,
            upper?node->zh:mz
        );

        if(children[i]==NULL) {
            for(size_t j=0;j<=i;++j)free_node(children[j]);
            return false;
        }
    }

    for(size_t i=0;i<8;++i)node->children[i]=children[i];
    tree->node_count+=8;
    return true;
}

static bool append_point(
    OctNode *node,
    IntOctPoint point
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
    IntOctree *tree,
    OctNode *node,
    IntOctPoint point,
    size_t depth
);

static bool split_leaf(
    IntOctree *tree,
    OctNode *node,
    size_t depth,
    int64_t mx,int64_t my,int64_t mz
) {
    if(!create_children(tree,node,mx,my,mz))return false;

    const size_t old_count=node->point_count;
    size_t octant_counts[8]={0};

    for(size_t i=0;i<old_count;++i) {
        const size_t q=octant(mx,my,mz,&node->points[i]);
        ++octant_counts[q];
    }

    for(size_t i=0;i<8;++i) {
        if(octant_counts[i]>0 &&
           !reserve_points(
               node->children[i],octant_counts[i]
           )) {
            for(size_t j=0;j<8;++j) {
                free_node(node->children[j]);
                node->children[j]=NULL;
            }
            tree->node_count-=8;
            return false;
        }
    }

    IntOctPoint *old=node->points;

    node->points=NULL;
    node->point_count=0;
    node->point_capacity=0;
    node->subtree_size=0;

    for(size_t i=0;i<old_count;++i) {
        const size_t q=octant(mx,my,mz,&old[i]);

        if(!insert_rec(
                tree,node->children[q],old[i],depth+1
            )) {
            free(old);
            return false;
        }

        ++node->subtree_size;
    }

    free(old);
    return true;
}

static bool insert_rec(
    IntOctree *tree,
    OctNode *node,
    IntOctPoint point,
    size_t depth
) {
    if(!point_inside(node,&point))return false;

    if(is_leaf(node)) {
        if(node->point_count<tree->bucket_capacity||
           depth>=tree->max_depth) {
            return append_point(node,point);
        }

        int64_t mx=0,my=0,mz=0;

        if(!split_mids(node,&mx,&my,&mz)) {
            return append_point(node,point);
        }

        if(!split_leaf(
                tree,node,depth,mx,my,mz
            )) {
            return false;
        }
    }

    int64_t mx=0,my=0,mz=0;

    if(!split_mids(node,&mx,&my,&mz))return false;

    const size_t q=octant(mx,my,mz,&point);

    if(!insert_rec(
            tree,node->children[q],point,depth+1
        )) {
        return false;
    }

    ++node->subtree_size;
    return true;
}

bool int_octree_insert(
    IntOctree *tree,
    IntOctPoint point
) {
    if(tree==NULL||tree->size==SIZE_MAX||
       !point_inside(tree->root,&point)) {
        return false;
    }

    if(!insert_rec(tree,tree->root,point,0)) {
        return false;
    }

    ++tree->size;
    return true;
}

static bool box_disjoint(
    const OctNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    return node->xh<=xl||xh<=node->xl||
           node->yh<=yl||yh<=node->yl||
           node->zh<=zl||zh<=node->zl;
}

static bool box_inside(
    const OctNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    return xl<=node->xl&&node->xh<=xh&&
           yl<=node->yl&&node->yh<=yh&&
           zl<=node->zl&&node->zh<=zh;
}

static bool point_query(
    const IntOctPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh&&
           zl<=p->z&&p->z<zh;
}

static size_t count_rec(
    const OctNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh
) {
    if(box_disjoint(node,xl,xh,yl,yh,zl,zh))return 0;

    if(box_inside(node,xl,xh,yl,yh,zl,zh)) {
        return node->subtree_size;
    }

    if(is_leaf(node)) {
        size_t count=0;

        for(size_t i=0;i<node->point_count;++i) {
            if(point_query(
                    &node->points[i],
                    xl,xh,yl,yh,zl,zh
                )) {
                ++count;
            }
        }

        return count;
    }

    size_t count=0;

    for(size_t i=0;i<8;++i) {
        count+=count_rec(
            node->children[i],
            xl,xh,yl,yh,zl,zh
        );
    }

    return count;
}

bool int_octree_query_count(
    const IntOctree *tree,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh,
    size_t *out_count
) {
    if(tree==NULL||out_count==NULL||
       xl>=xh||yl>=yh||zl>=zh) {
        return false;
    }

    *out_count=count_rec(
        tree->root,xl,xh,yl,yh,zl,zh
    );
    return true;
}

static bool collect_all(
    const OctNode *node,
    IntOctPoint *output,
    size_t capacity,
    size_t *written
) {
    if(is_leaf(node)) {
        if(node->point_count>capacity-*written)return false;

        for(size_t i=0;i<node->point_count;++i) {
            output[(*written)++]=node->points[i];
        }

        return true;
    }

    for(size_t i=0;i<8;++i) {
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
    const OctNode *node,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh,
    IntOctPoint *output,
    size_t capacity,
    size_t *written
) {
    if(box_disjoint(node,xl,xh,yl,yh,zl,zh))return true;

    if(box_inside(node,xl,xh,yl,yh,zl,zh)) {
        return collect_all(node,output,capacity,written);
    }

    if(is_leaf(node)) {
        for(size_t i=0;i<node->point_count;++i) {
            if(point_query(
                    &node->points[i],
                    xl,xh,yl,yh,zl,zh
                )) {
                if(*written>=capacity)return false;
                output[(*written)++]=node->points[i];
            }
        }
        return true;
    }

    for(size_t i=0;i<8;++i) {
        if(!report_rec(
                node->children[i],
                xl,xh,yl,yh,zl,zh,
                output,capacity,written
            )) {
            return false;
        }
    }

    return true;
}

bool int_octree_query_report(
    const IntOctree *tree,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    int64_t zl,int64_t zh,
    IntOctPoint *output,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL||
       xl>=xh||yl>=yh||zl>=zh) {
        return false;
    }

    size_t needed=0;

    if(!int_octree_query_count(
            tree,xl,xh,yl,yh,zl,zh,&needed
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
            xl,xh,yl,yh,zl,zh,
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

static ValidateResult validate_rec(
    const OctNode *node,
    size_t depth,
    size_t max_depth
) {
    if(node==NULL||node->xl>=node->xh||
       node->yl>=node->yh||node->zl>=node->zh||
       depth>max_depth) {
        return (ValidateResult){.ok=false};
    }

    if(is_leaf(node)) {
        for(size_t i=0;i<node->point_count;++i) {
            if(!point_inside(node,&node->points[i])) {
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

    int64_t mx=0,my=0,mz=0;

    if(!split_mids(node,&mx,&my,&mz)) {
        return (ValidateResult){.ok=false};
    }

    size_t points=0;
    size_t nodes=1;

    for(size_t i=0;i<8;++i) {
        const bool east=(i&1U)!=0U;
        const bool north=(i&2U)!=0U;
        const bool upper=(i&4U)!=0U;

        const OctNode *child=node->children[i];

        if(child==NULL||
           child->xl!=(east?mx:node->xl)||
           child->xh!=(east?node->xh:mx)||
           child->yl!=(north?my:node->yl)||
           child->yh!=(north?node->yh:my)||
           child->zl!=(upper?mz:node->zl)||
           child->zh!=(upper?node->zh:mz)) {
            return (ValidateResult){.ok=false};
        }

        ValidateResult result=validate_rec(
            child,depth+1,max_depth
        );

        if(!result.ok)return result;

        points+=result.points;
        nodes+=result.nodes;
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

bool int_octree_validate(const IntOctree *tree) {
    if(tree==NULL||tree->root==NULL)return false;

    ValidateResult result=validate_rec(
        tree->root,0,tree->max_depth
    );

    return result.ok&&
           result.points==tree->size&&
           result.nodes==tree->node_count;
}
