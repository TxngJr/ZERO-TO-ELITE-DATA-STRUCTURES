#include "int_r_tree.h"

#include <float.h>
#include <stdint.h>
#include <stdlib.h>

enum {
    RTREE_MAX_ENTRIES=4,
    RTREE_MIN_ENTRIES=2,
    RTREE_OVERFLOW_ENTRIES=5
};

typedef struct {
    int64_t xl,xh,yl,yh;
} Rect;

typedef struct RNode {
    bool leaf;
    size_t count;
    Rect rects[RTREE_OVERFLOW_ENTRIES];
    uint64_t ids[RTREE_OVERFLOW_ENTRIES];
    struct RNode *children[RTREE_OVERFLOW_ENTRIES];
} RNode;

struct IntRTree {
    RNode *root;
    size_t size;
    size_t node_count;
};

static bool rect_valid(Rect r) {
    return r.xl<r.xh&&r.yl<r.yh;
}

static Rect object_rect(IntRRect r) {
    return (Rect){
        .xl=r.x_low,.xh=r.x_high,
        .yl=r.y_low,.yh=r.y_high
    };
}

static Rect rect_combine(Rect a,Rect b) {
    return (Rect){
        .xl=a.xl<b.xl?a.xl:b.xl,
        .xh=a.xh>b.xh?a.xh:b.xh,
        .yl=a.yl<b.yl?a.yl:b.yl,
        .yh=a.yh>b.yh?a.yh:b.yh
    };
}

static bool rect_equal(Rect a,Rect b) {
    return a.xl==b.xl&&a.xh==b.xh&&
           a.yl==b.yl&&a.yh==b.yh;
}

static bool rect_overlap(Rect a,Rect b) {
    return a.xl<b.xh&&b.xl<a.xh&&
           a.yl<b.yh&&b.yl<a.yh;
}

static long double rect_area(Rect r) {
    const long double width=
        (long double)r.xh-(long double)r.xl;
    const long double height=
        (long double)r.yh-(long double)r.yl;

    return width*height;
}

static long double enlargement(Rect current,Rect added) {
    return rect_area(rect_combine(current,added))-
           rect_area(current);
}

static RNode *make_node(bool leaf) {
    RNode *node=calloc(1,sizeof *node);
    if(node!=NULL)node->leaf=leaf;
    return node;
}

IntRTree *int_r_tree_create(void) {
    IntRTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->root=make_node(true);

    if(tree->root==NULL) {
        free(tree);
        return NULL;
    }

    tree->node_count=1;
    return tree;
}

static void free_node(RNode *node) {
    if(node==NULL)return;

    if(!node->leaf) {
        for(size_t i=0;i<node->count;++i) {
            free_node(node->children[i]);
        }
    }

    free(node);
}

void int_r_tree_free(IntRTree *tree) {
    if(tree==NULL)return;
    free_node(tree->root);
    free(tree);
}

size_t int_r_tree_size(const IntRTree *tree) {
    return tree==NULL?0:tree->size;
}

size_t int_r_tree_node_count(const IntRTree *tree) {
    return tree==NULL?0:tree->node_count;
}

static Rect node_mbr(const RNode *node) {
    Rect result=node->rects[0];

    for(size_t i=1;i<node->count;++i) {
        result=rect_combine(result,node->rects[i]);
    }

    return result;
}

static void add_entry(
    RNode *node,
    Rect rect,
    uint64_t id,
    RNode *child
) {
    const size_t i=node->count++;

    node->rects[i]=rect;
    node->ids[i]=id;
    node->children[i]=child;
}

static size_t choose_child(
    const RNode *node,
    Rect rect
) {
    size_t best=0;
    long double best_enlargement=LDBL_MAX;
    long double best_area=LDBL_MAX;
    size_t best_count=SIZE_MAX;

    for(size_t i=0;i<node->count;++i) {
        const long double e=enlargement(
            node->rects[i],rect
        );
        const long double a=rect_area(node->rects[i]);
        const size_t c=node->children[i]->count;

        if(e<best_enlargement||
           (e==best_enlargement&&a<best_area)||
           (e==best_enlargement&&a==best_area&&
            c<best_count)) {
            best=i;
            best_enlargement=e;
            best_area=a;
            best_count=c;
        }
    }

    return best;
}

static void pick_seeds(
    const Rect rects[RTREE_OVERFLOW_ENTRIES],
    size_t *out_a,
    size_t *out_b
) {
    long double best=-LDBL_MAX;
    size_t a=0;
    size_t b=1;

    for(size_t i=0;i<RTREE_OVERFLOW_ENTRIES;++i) {
        for(size_t j=i+1;j<RTREE_OVERFLOW_ENTRIES;++j) {
            const long double waste=
                rect_area(rect_combine(rects[i],rects[j]))-
                rect_area(rects[i])-
                rect_area(rects[j]);

            if(waste>best) {
                best=waste;
                a=i;
                b=j;
            }
        }
    }

    *out_a=a;
    *out_b=b;
}

static void assign_temp(
    RNode *group,
    const Rect *rects,
    const uint64_t *ids,
    RNode *const *children,
    size_t index
) {
    add_entry(
        group,
        rects[index],
        ids[index],
        children[index]
    );
}

static void split_with_extra(
    IntRTree *tree,
    RNode *node,
    Rect extra_rect,
    uint64_t extra_id,
    RNode *extra_child,
    RNode *sibling,
    RNode **out_sibling
) {
    Rect rects[RTREE_OVERFLOW_ENTRIES];
    uint64_t ids[RTREE_OVERFLOW_ENTRIES]={0};
    RNode *children[RTREE_OVERFLOW_ENTRIES]={0};

    for(size_t i=0;i<RTREE_MAX_ENTRIES;++i) {
        rects[i]=node->rects[i];
        ids[i]=node->ids[i];
        children[i]=node->children[i];
    }

    rects[RTREE_MAX_ENTRIES]=extra_rect;
    ids[RTREE_MAX_ENTRIES]=extra_id;
    children[RTREE_MAX_ENTRIES]=extra_child;

    node->count=0;
    sibling->leaf=node->leaf;
    sibling->count=0;

    bool assigned[RTREE_OVERFLOW_ENTRIES]={false};

    size_t seed_a=0;
    size_t seed_b=0;
    pick_seeds(rects,&seed_a,&seed_b);

    assign_temp(
        node,rects,ids,children,seed_a
    );
    assign_temp(
        sibling,rects,ids,children,seed_b
    );

    assigned[seed_a]=true;
    assigned[seed_b]=true;
    size_t remaining=RTREE_OVERFLOW_ENTRIES-2;

    while(remaining>0) {
        if(node->count+remaining==RTREE_MIN_ENTRIES) {
            for(size_t i=0;i<RTREE_OVERFLOW_ENTRIES;++i) {
                if(!assigned[i]) {
                    assign_temp(node,rects,ids,children,i);
                    assigned[i]=true;
                    --remaining;
                }
            }
            break;
        }

        if(sibling->count+remaining==RTREE_MIN_ENTRIES) {
            for(size_t i=0;i<RTREE_OVERFLOW_ENTRIES;++i) {
                if(!assigned[i]) {
                    assign_temp(sibling,rects,ids,children,i);
                    assigned[i]=true;
                    --remaining;
                }
            }
            break;
        }

        const Rect mbr_a=node_mbr(node);
        const Rect mbr_b=node_mbr(sibling);

        size_t selected=SIZE_MAX;
        long double selected_diff=-1.0L;
        long double selected_ea=0.0L;
        long double selected_eb=0.0L;

        for(size_t i=0;i<RTREE_OVERFLOW_ENTRIES;++i) {
            if(assigned[i])continue;

            const long double ea=enlargement(mbr_a,rects[i]);
            const long double eb=enlargement(mbr_b,rects[i]);
            const long double diff=
                ea>eb?ea-eb:eb-ea;

            if(selected==SIZE_MAX||diff>selected_diff) {
                selected=i;
                selected_diff=diff;
                selected_ea=ea;
                selected_eb=eb;
            }
        }

        RNode *target=NULL;

        if(selected_ea<selected_eb) {
            target=node;
        } else if(selected_eb<selected_ea) {
            target=sibling;
        } else {
            const long double area_a=rect_area(mbr_a);
            const long double area_b=rect_area(mbr_b);

            if(area_a<area_b)target=node;
            else if(area_b<area_a)target=sibling;
            else if(node->count<=sibling->count)target=node;
            else target=sibling;
        }

        assign_temp(
            target,rects,ids,children,selected
        );

        assigned[selected]=true;
        --remaining;
    }

    ++tree->node_count;
    *out_sibling=sibling;
}

static bool insert_rec(
    IntRTree *tree,
    RNode *node,
    Rect rect,
    uint64_t id,
    RNode **out_split
) {
    *out_split=NULL;

    if(node->leaf) {
        if(node->count<RTREE_MAX_ENTRIES) {
            add_entry(node,rect,id,NULL);
            return true;
        }

        RNode *sibling=make_node(true);
        if(sibling==NULL)return false;

        split_with_extra(
            tree,node,rect,id,NULL,
            sibling,out_split
        );

        return true;
    }

    RNode *parent_spare=NULL;

    if(node->count==RTREE_MAX_ENTRIES) {
        parent_spare=make_node(false);

        if(parent_spare==NULL)return false;
    }

    const size_t index=choose_child(node,rect);
    RNode *child_split=NULL;

    if(!insert_rec(
            tree,
            node->children[index],
            rect,id,
            &child_split
        )) {
        free(parent_spare);
        return false;
    }

    node->rects[index]=
        node_mbr(node->children[index]);

    if(child_split==NULL) {
        free(parent_spare);
        return true;
    }

    const Rect split_rect=node_mbr(child_split);

    if(node->count<RTREE_MAX_ENTRIES) {
        add_entry(
            node,split_rect,0,child_split
        );
        free(parent_spare);
        return true;
    }

    split_with_extra(
        tree,node,
        split_rect,0,child_split,
        parent_spare,out_split
    );

    return true;
}

bool int_r_tree_insert(
    IntRTree *tree,
    IntRRect rectangle
) {
    if(tree==NULL||tree->size==SIZE_MAX)return false;

    const Rect rect=object_rect(rectangle);

    if(!rect_valid(rect))return false;

    RNode *new_root_spare=NULL;

    if(tree->root->count==RTREE_MAX_ENTRIES) {
        new_root_spare=make_node(false);

        if(new_root_spare==NULL)return false;
    }

    RNode *split=NULL;

    if(!insert_rec(
            tree,tree->root,
            rect,rectangle.id,
            &split
        )) {
        free(new_root_spare);
        return false;
    }

    if(split!=NULL) {
        RNode *old_root=tree->root;

        if(new_root_spare==NULL) {
            /*
             * This cannot happen: only a full root can split.
             */
            return false;
        }

        add_entry(
            new_root_spare,
            node_mbr(old_root),
            0,
            old_root
        );

        add_entry(
            new_root_spare,
            node_mbr(split),
            0,
            split
        );

        tree->root=new_root_spare;
        ++tree->node_count;
    } else {
        free(new_root_spare);
    }

    ++tree->size;
    return true;
}

static size_t query_count_rec(
    const RNode *node,
    Rect query
) {
    size_t count=0;

    if(node->leaf) {
        for(size_t i=0;i<node->count;++i) {
            if(rect_overlap(node->rects[i],query)) {
                ++count;
            }
        }

        return count;
    }

    for(size_t i=0;i<node->count;++i) {
        if(rect_overlap(node->rects[i],query)) {
            count+=query_count_rec(
                node->children[i],query
            );
        }
    }

    return count;
}

bool int_r_tree_query_count(
    const IntRTree *tree,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    size_t *out_count
) {
    if(tree==NULL||out_count==NULL) {
        return false;
    }

    const Rect query={xl,xh,yl,yh};

    if(!rect_valid(query))return false;

    *out_count=query_count_rec(tree->root,query);
    return true;
}

static bool query_report_rec(
    const RNode *node,
    Rect query,
    IntRRect *output,
    size_t capacity,
    size_t *written
) {
    if(node->leaf) {
        for(size_t i=0;i<node->count;++i) {
            if(!rect_overlap(node->rects[i],query))continue;

            if(*written>=capacity)return false;

            output[(*written)++]=(IntRRect){
                .x_low=node->rects[i].xl,
                .x_high=node->rects[i].xh,
                .y_low=node->rects[i].yl,
                .y_high=node->rects[i].yh,
                .id=node->ids[i]
            };
        }

        return true;
    }

    for(size_t i=0;i<node->count;++i) {
        if(rect_overlap(node->rects[i],query)) {
            if(!query_report_rec(
                    node->children[i],
                    query,
                    output,capacity,written
                )) {
                return false;
            }
        }
    }

    return true;
}

bool int_r_tree_query_report(
    const IntRTree *tree,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    IntRRect *output,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL)return false;

    size_t needed=0;

    if(!int_r_tree_query_count(
            tree,xl,xh,yl,yh,&needed
        )) {
        return false;
    }

    if(needed>capacity||
       (needed>0&&output==NULL)) {
        return false;
    }

    const Rect query={xl,xh,yl,yh};
    size_t written=0;

    if(!query_report_rec(
            tree->root,query,
            output,capacity,&written
        )) {
        return false;
    }

    *out_written=written;
    return written==needed;
}

typedef struct {
    bool ok;
    size_t objects;
    size_t nodes;
    size_t leaf_depth;
    bool has_leaf_depth;
    Rect mbr;
    bool has_mbr;
} ValidateResult;

static ValidateResult validate_rec(
    const RNode *node,
    bool is_root,
    size_t depth
) {
    if(node==NULL||node->count>RTREE_MAX_ENTRIES) {
        return (ValidateResult){.ok=false};
    }

    if(is_root) {
        if(!node->leaf&&node->count<2) {
            return (ValidateResult){.ok=false};
        }
    } else if(node->count<RTREE_MIN_ENTRIES) {
        return (ValidateResult){.ok=false};
    }

    if(node->leaf) {
        Rect mbr={0};
        bool has=false;

        for(size_t i=0;i<node->count;++i) {
            if(!rect_valid(node->rects[i])||
               node->children[i]!=NULL) {
                return (ValidateResult){.ok=false};
            }

            mbr=has
                ?rect_combine(mbr,node->rects[i])
                :node->rects[i];
            has=true;
        }

        return (ValidateResult){
            .ok=true,
            .objects=node->count,
            .nodes=1,
            .leaf_depth=depth,
            .has_leaf_depth=true,
            .mbr=mbr,
            .has_mbr=has
        };
    }

    size_t objects=0;
    size_t nodes=1;
    size_t leaf_depth=SIZE_MAX;
    Rect mbr={0};
    bool has_mbr=false;

    for(size_t i=0;i<node->count;++i) {
        if(node->children[i]==NULL||
           !rect_valid(node->rects[i])) {
            return (ValidateResult){.ok=false};
        }

        ValidateResult child=validate_rec(
            node->children[i],false,depth+1
        );

        if(!child.ok||!child.has_mbr||
           !rect_equal(node->rects[i],child.mbr)) {
            return (ValidateResult){.ok=false};
        }

        if(leaf_depth==SIZE_MAX) {
            leaf_depth=child.leaf_depth;
        } else if(leaf_depth!=child.leaf_depth) {
            return (ValidateResult){.ok=false};
        }

        objects+=child.objects;
        nodes+=child.nodes;
        mbr=has_mbr
            ?rect_combine(mbr,child.mbr)
            :child.mbr;
        has_mbr=true;
    }

    return (ValidateResult){
        .ok=true,
        .objects=objects,
        .nodes=nodes,
        .leaf_depth=leaf_depth,
        .has_leaf_depth=true,
        .mbr=mbr,
        .has_mbr=has_mbr
    };
}

bool int_r_tree_validate(const IntRTree *tree) {
    if(tree==NULL||tree->root==NULL)return false;

    ValidateResult result=validate_rec(
        tree->root,true,0
    );

    if(!result.ok||
       result.objects!=tree->size||
       result.nodes!=tree->node_count) {
        return false;
    }

    if(tree->size==0) {
        return tree->root->leaf&&tree->root->count==0;
    }

    return result.has_mbr;
}
