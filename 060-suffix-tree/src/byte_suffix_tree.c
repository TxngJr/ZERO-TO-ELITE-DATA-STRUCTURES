#include "byte_suffix_tree.h"

#include <stdint.h>
#include <stdlib.h>

enum { SUFFIX_SENTINEL=256 };

typedef struct {
    size_t start;
    size_t end;
    size_t child;
} SuffixEdge;

typedef struct {
    SuffixEdge *edges;
    size_t edge_count;
    size_t edge_capacity;
    size_t suffix_start;
    size_t leaf_count;
} SuffixNode;

struct ByteSuffixTree {
    uint16_t *symbols;
    size_t text_length;
    size_t symbol_count;

    SuffixNode *nodes;
    size_t node_count;
    size_t node_capacity;
    size_t root;
};

static bool reserve_edges(
    SuffixNode *node,
    size_t needed
) {
    if(needed<=node->edge_capacity)return true;

    size_t capacity=node->edge_capacity==0?4:node->edge_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *node->edges)return false;

    SuffixEdge *next=realloc(
        node->edges,capacity*sizeof *next
    );

    if(next==NULL)return false;

    node->edges=next;
    node->edge_capacity=capacity;
    return true;
}

static void init_node(
    SuffixNode *node,
    size_t suffix_start
) {
    *node=(SuffixNode){
        .suffix_start=suffix_start
    };
}

static size_t append_node(
    ByteSuffixTree *tree,
    size_t suffix_start
) {
    if(tree->node_count>=tree->node_capacity) {
        return SIZE_MAX;
    }

    const size_t index=tree->node_count++;
    init_node(&tree->nodes[index],suffix_start);
    return index;
}

static size_t find_edge(
    const ByteSuffixTree *tree,
    size_t node_index,
    uint16_t first_symbol
) {
    const SuffixNode *node=&tree->nodes[node_index];

    for(size_t i=0;i<node->edge_count;++i) {
        const SuffixEdge *edge=&node->edges[i];

        if(tree->symbols[edge->start]==first_symbol) {
            return i;
        }
    }

    return SIZE_MAX;
}

static bool append_edge(
    SuffixNode *node,
    SuffixEdge edge
) {
    if(node->edge_count==SIZE_MAX||
       !reserve_edges(node,node->edge_count+1)) {
        return false;
    }

    node->edges[node->edge_count++]=edge;
    return true;
}

static bool insert_suffix(
    ByteSuffixTree *tree,
    size_t suffix_start
) {
    size_t node_index=tree->root;
    size_t pos=suffix_start;

    while(pos<tree->symbol_count) {
        size_t edge_index=find_edge(
            tree,node_index,tree->symbols[pos]
        );

        if(edge_index==SIZE_MAX) {
            SuffixNode *node=&tree->nodes[node_index];

            if(!reserve_edges(node,node->edge_count+1)) {
                return false;
            }

            const size_t leaf=append_node(tree,suffix_start);

            if(leaf==SIZE_MAX)return false;

            node=&tree->nodes[node_index];
            node->edges[node->edge_count++]=(SuffixEdge){
                .start=pos,
                .end=tree->symbol_count,
                .child=leaf
            };

            return true;
        }

        const SuffixEdge old=
            tree->nodes[node_index].edges[edge_index];

        size_t matched=0;

        while(old.start+matched<old.end&&
              pos+matched<tree->symbol_count&&
              tree->symbols[old.start+matched]==
                  tree->symbols[pos+matched]) {
            ++matched;
        }

        if(old.start+matched==old.end) {
            node_index=old.child;
            pos+=matched;
            continue;
        }

        /*
         * With unique sentinel, a suffix cannot end before
         * producing a mismatch/branch here.
         */
        if(pos+matched>=tree->symbol_count) {
            return false;
        }

        const size_t old_count=tree->node_count;

        const size_t split=append_node(tree,SIZE_MAX);
        const size_t leaf=append_node(tree,suffix_start);

        if(split==SIZE_MAX||leaf==SIZE_MAX) {
            tree->node_count=old_count;
            return false;
        }

        SuffixNode *split_node=&tree->nodes[split];

        if(!reserve_edges(split_node,2)) {
            free(split_node->edges);
            tree->node_count=old_count;
            return false;
        }

        split_node=&tree->nodes[split];
        split_node->edges[0]=(SuffixEdge){
            .start=old.start+matched,
            .end=old.end,
            .child=old.child
        };
        split_node->edges[1]=(SuffixEdge){
            .start=pos+matched,
            .end=tree->symbol_count,
            .child=leaf
        };
        split_node->edge_count=2;

        tree->nodes[node_index].edges[edge_index]=
            (SuffixEdge){
                .start=old.start,
                .end=old.start+matched,
                .child=split
            };

        return true;
    }

    return false;
}

static size_t compute_leaf_counts(
    ByteSuffixTree *tree,
    size_t node_index
) {
    SuffixNode *node=&tree->nodes[node_index];

    if(node->edge_count==0) {
        node->leaf_count=1;
        return 1;
    }

    size_t total=0;

    for(size_t i=0;i<node->edge_count;++i) {
        total+=compute_leaf_counts(
            tree,node->edges[i].child
        );
    }

    node->leaf_count=total;
    return total;
}

ByteSuffixTree *byte_suffix_tree_create(
    const uint8_t *text,
    size_t length
) {
    if(length>0&&text==NULL)return NULL;
    if(length==SIZE_MAX)return NULL;

    const size_t symbols=length+1;

    if(symbols>SIZE_MAX/sizeof(uint16_t)||
       symbols>(SIZE_MAX-1)/2) {
        return NULL;
    }

    ByteSuffixTree *tree=calloc(1,sizeof *tree);
    if(tree==NULL)return NULL;

    tree->text_length=length;
    tree->symbol_count=symbols;
    tree->root=0;
    tree->node_capacity=2*symbols+1;

    tree->symbols=malloc(symbols*sizeof *tree->symbols);
    tree->nodes=calloc(
        tree->node_capacity,
        sizeof *tree->nodes
    );

    if(tree->symbols==NULL||tree->nodes==NULL) {
        byte_suffix_tree_free(tree);
        return NULL;
    }

    for(size_t i=0;i<length;++i) {
        tree->symbols[i]=(uint16_t)text[i];
    }

    tree->symbols[length]=SUFFIX_SENTINEL;

    tree->node_count=1;
    init_node(&tree->nodes[0],SIZE_MAX);

    for(size_t start=0;start<symbols;++start) {
        if(!insert_suffix(tree,start)) {
            byte_suffix_tree_free(tree);
            return NULL;
        }
    }

    if(compute_leaf_counts(tree,tree->root)!=symbols) {
        byte_suffix_tree_free(tree);
        return NULL;
    }

    return tree;
}

void byte_suffix_tree_free(ByteSuffixTree *tree) {
    if(tree==NULL)return;

    if(tree->nodes!=NULL) {
        for(size_t i=0;i<tree->node_count;++i) {
            free(tree->nodes[i].edges);
        }
    }

    free(tree->nodes);
    free(tree->symbols);
    free(tree);
}

size_t byte_suffix_tree_text_length(const ByteSuffixTree *tree) {
    return tree==NULL?0:tree->text_length;
}

size_t byte_suffix_tree_node_count(const ByteSuffixTree *tree) {
    return tree==NULL?0:tree->node_count;
}

static bool locate_pattern(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_locus
) {
    if(tree==NULL||pattern==NULL||
       pattern_length==0||out_locus==NULL) {
        return false;
    }

    size_t node_index=tree->root;
    size_t pos=0;

    while(pos<pattern_length) {
        const size_t edge_index=find_edge(
            tree,node_index,(uint16_t)pattern[pos]
        );

        if(edge_index==SIZE_MAX)return false;

        const SuffixEdge edge=
            tree->nodes[node_index].edges[edge_index];

        size_t k=0;

        while(edge.start+k<edge.end&&pos<pattern_length) {
            const uint16_t symbol=tree->symbols[edge.start+k];

            if(symbol>255U||
               symbol!=(uint16_t)pattern[pos]) {
                return false;
            }

            ++k;
            ++pos;
        }

        if(pos==pattern_length) {
            *out_locus=edge.child;
            return true;
        }

        if(edge.start+k!=edge.end)return false;

        node_index=edge.child;
    }

    *out_locus=node_index;
    return true;
}

bool byte_suffix_tree_contains(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length
) {
    size_t locus=0;
    return locate_pattern(
        tree,pattern,pattern_length,&locus
    );
}

bool byte_suffix_tree_count(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
) {
    if(out_count==NULL)return false;

    size_t locus=0;

    if(!locate_pattern(
            tree,pattern,pattern_length,&locus
        )) {
        *out_count=0;
        return true;
    }

    *out_count=tree->nodes[locus].leaf_count;
    return true;
}

static bool report_leaves(
    const ByteSuffixTree *tree,
    size_t node_index,
    size_t *positions,
    size_t capacity,
    size_t *written
) {
    const SuffixNode *node=&tree->nodes[node_index];

    if(node->edge_count==0) {
        if(node->suffix_start>=tree->text_length) {
            return true;
        }

        if(*written>=capacity)return false;
        positions[(*written)++]=node->suffix_start;
        return true;
    }

    for(size_t i=0;i<node->edge_count;++i) {
        if(!report_leaves(
                tree,node->edges[i].child,
                positions,capacity,written
            )) {
            return false;
        }
    }

    return true;
}

bool byte_suffix_tree_report(
    const ByteSuffixTree *tree,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
) {
    if(tree==NULL||out_written==NULL||
       pattern==NULL||pattern_length==0) {
        return false;
    }

    size_t locus=0;

    if(!locate_pattern(
            tree,pattern,pattern_length,&locus
        )) {
        *out_written=0;
        return true;
    }

    const size_t needed=tree->nodes[locus].leaf_count;

    /*
     * Nonempty byte patterns cannot match sentinel-only leaf,
     * so needed equals reportable leaf count.
     */
    if(needed>capacity||(needed>0&&positions==NULL)) {
        return false;
    }

    size_t written=0;

    if(!report_leaves(
            tree,locus,positions,capacity,&written
        )) {
        return false;
    }

    *out_written=written;
    return written==needed;
}

typedef struct {
    bool ok;
    size_t nodes;
    size_t leaves;
} ValidateResult;

static ValidateResult validate_rec(
    const ByteSuffixTree *tree,
    size_t node_index,
    bool *seen,
    bool *suffix_seen
) {
    if(node_index>=tree->node_count||seen[node_index]) {
        return (ValidateResult){.ok=false};
    }

    seen[node_index]=true;

    const SuffixNode *node=&tree->nodes[node_index];

    if(node->edge_count==0) {
        if(node->suffix_start>=tree->symbol_count||
           suffix_seen[node->suffix_start]||
           node->leaf_count!=1) {
            return (ValidateResult){.ok=false};
        }

        suffix_seen[node->suffix_start]=true;

        return (ValidateResult){
            .ok=true,.nodes=1,.leaves=1
        };
    }

    if(node->suffix_start!=SIZE_MAX||
       node->leaf_count==0) {
        return (ValidateResult){.ok=false};
    }

    bool first_seen[257]={false};
    size_t nodes=1;
    size_t leaves=0;

    for(size_t i=0;i<node->edge_count;++i) {
        const SuffixEdge edge=node->edges[i];

        if(edge.start>=edge.end||
           edge.end>tree->symbol_count||
           edge.child>=tree->node_count) {
            return (ValidateResult){.ok=false};
        }

        const uint16_t first=tree->symbols[edge.start];

        if(first>SUFFIX_SENTINEL||first_seen[first]) {
            return (ValidateResult){.ok=false};
        }

        first_seen[first]=true;

        ValidateResult child=validate_rec(
            tree,edge.child,seen,suffix_seen
        );

        if(!child.ok)return child;

        nodes+=child.nodes;
        leaves+=child.leaves;
    }

    if(node->leaf_count!=leaves) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,.nodes=nodes,.leaves=leaves
    };
}

static bool exact_suffix_exists(
    const ByteSuffixTree *tree,
    size_t suffix_start
) {
    size_t node_index=tree->root;
    size_t pos=suffix_start;

    while(pos<tree->symbol_count) {
        const size_t edge_index=find_edge(
            tree,node_index,tree->symbols[pos]
        );

        if(edge_index==SIZE_MAX)return false;

        const SuffixEdge edge=
            tree->nodes[node_index].edges[edge_index];

        for(size_t k=edge.start;k<edge.end;++k) {
            if(pos>=tree->symbol_count||
               tree->symbols[pos]!=tree->symbols[k]) {
                return false;
            }
            ++pos;
        }

        node_index=edge.child;
    }

    return tree->nodes[node_index].edge_count==0&&
           tree->nodes[node_index].suffix_start==suffix_start;
}

bool byte_suffix_tree_validate(const ByteSuffixTree *tree) {
    if(tree==NULL||tree->symbols==NULL||
       tree->nodes==NULL||
       tree->symbol_count!=tree->text_length+1||
       tree->symbols[tree->text_length]!=SUFFIX_SENTINEL||
       tree->root>=tree->node_count||
       tree->node_count>tree->node_capacity) {
        return false;
    }

    bool *seen=calloc(tree->node_count,sizeof *seen);
    bool *suffix_seen=calloc(
        tree->symbol_count,sizeof *suffix_seen
    );

    if(seen==NULL||suffix_seen==NULL) {
        free(seen);
        free(suffix_seen);
        return false;
    }

    ValidateResult result=validate_rec(
        tree,tree->root,seen,suffix_seen
    );

    if(!result.ok||
       result.nodes!=tree->node_count||
       result.leaves!=tree->symbol_count) {
        free(seen);
        free(suffix_seen);
        return false;
    }

    for(size_t s=0;s<tree->symbol_count;++s) {
        if(!suffix_seen[s]||
           !exact_suffix_exists(tree,s)) {
            free(seen);
            free(suffix_seen);
            return false;
        }
    }

    free(seen);
    free(suffix_seen);
    return true;
}
