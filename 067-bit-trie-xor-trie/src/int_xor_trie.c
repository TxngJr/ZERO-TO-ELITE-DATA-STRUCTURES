#include "int_xor_trie.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t child[2];
    size_t subtree_count;
    size_t terminal_count;
} XorNode;

struct IntXorTrie {
    XorNode *nodes;
    size_t node_count;
    size_t node_capacity;
    size_t size;
};

static void init_node(XorNode *node) {
    *node=(XorNode){
        .child={SIZE_MAX,SIZE_MAX}
    };
}

static bool reserve_nodes(
    IntXorTrie *trie,
    size_t needed
) {
    if(needed<=trie->node_capacity)return true;

    size_t capacity=trie->node_capacity==0
        ?8:trie->node_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *trie->nodes)return false;

    XorNode *next=realloc(
        trie->nodes,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    trie->nodes=next;
    trie->node_capacity=capacity;
    return true;
}

static size_t append_node_noalloc(IntXorTrie *trie) {
    const size_t index=trie->node_count++;
    init_node(&trie->nodes[index]);
    return index;
}

IntXorTrie *int_xor_trie_create(void) {
    IntXorTrie *trie=calloc(1,sizeof *trie);
    if(trie==NULL)return NULL;

    if(!reserve_nodes(trie,1)) {
        free(trie);
        return NULL;
    }

    append_node_noalloc(trie);
    return trie;
}

void int_xor_trie_free(IntXorTrie *trie) {
    if(trie==NULL)return;
    free(trie->nodes);
    free(trie);
}

size_t int_xor_trie_size(const IntXorTrie *trie) {
    return trie==NULL?0:trie->size;
}

size_t int_xor_trie_node_count(const IntXorTrie *trie) {
    return trie==NULL?0:trie->node_count;
}

bool int_xor_trie_insert(IntXorTrie *trie,uint64_t value) {
    if(trie==NULL||trie->size==SIZE_MAX||
       trie->node_count>SIZE_MAX-64U) {
        return false;
    }

    if(!reserve_nodes(trie,trie->node_count+64U)) {
        return false;
    }

    size_t node=0;
    ++trie->nodes[node].subtree_count;

    for(int bit=63;bit>=0;--bit) {
        const unsigned branch=
            (unsigned)((value>>(unsigned)bit)&UINT64_C(1));

        size_t child=trie->nodes[node].child[branch];

        if(child==SIZE_MAX) {
            child=append_node_noalloc(trie);
            trie->nodes[node].child[branch]=child;
        }

        node=child;
        ++trie->nodes[node].subtree_count;
    }

    ++trie->nodes[node].terminal_count;
    ++trie->size;
    return true;
}

static bool find_path(
    const IntXorTrie *trie,
    uint64_t value,
    size_t path[65]
) {
    if(trie==NULL||trie->node_count==0)return false;

    size_t node=0;
    path[0]=0;

    for(int bit=63,depth=1;bit>=0;--bit,++depth) {
        const unsigned branch=
            (unsigned)((value>>(unsigned)bit)&UINT64_C(1));

        const size_t child=trie->nodes[node].child[branch];

        if(child==SIZE_MAX||
           child>=trie->node_count||
           trie->nodes[child].subtree_count==0) {
            return false;
        }

        node=child;
        path[depth]=node;
    }

    return trie->nodes[node].terminal_count>0;
}

bool int_xor_trie_remove(IntXorTrie *trie,uint64_t value) {
    if(trie==NULL||trie->size==0)return false;

    size_t path[65];

    if(!find_path(trie,value,path))return false;

    for(size_t i=0;i<65;++i) {
        --trie->nodes[path[i]].subtree_count;
    }

    --trie->nodes[path[64]].terminal_count;
    --trie->size;
    return true;
}

bool int_xor_trie_contains(
    const IntXorTrie *trie,
    uint64_t value,
    bool *out_present
) {
    if(trie==NULL||out_present==NULL)return false;

    size_t path[65];
    *out_present=find_path(trie,value,path);
    return true;
}

static bool choose_xor(
    const IntXorTrie *trie,
    uint64_t query,
    bool maximize,
    uint64_t *out_value,
    uint64_t *out_xor
) {
    if(trie==NULL||out_value==NULL||out_xor==NULL||
       trie->size==0) {
        return false;
    }

    size_t node=0;
    uint64_t value=0;

    for(int bit=63;bit>=0;--bit) {
        const unsigned q=
            (unsigned)((query>>(unsigned)bit)&UINT64_C(1));

        const unsigned preferred=maximize?(q^1U):q;
        const unsigned fallback=preferred^1U;

        size_t child=trie->nodes[node].child[preferred];
        unsigned chosen=preferred;

        if(child==SIZE_MAX||
           trie->nodes[child].subtree_count==0) {
            child=trie->nodes[node].child[fallback];
            chosen=fallback;
        }

        if(child==SIZE_MAX||
           trie->nodes[child].subtree_count==0) {
            return false;
        }

        if(chosen!=0U) {
            value|=UINT64_C(1)<<(unsigned)bit;
        }

        node=child;
    }

    if(trie->nodes[node].terminal_count==0)return false;

    *out_value=value;
    *out_xor=value^query;
    return true;
}

bool int_xor_trie_min_xor(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t *out_value,
    uint64_t *out_xor
) {
    return choose_xor(
        trie,query,false,out_value,out_xor
    );
}

bool int_xor_trie_max_xor(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t *out_value,
    uint64_t *out_xor
) {
    return choose_xor(
        trie,query,true,out_value,out_xor
    );
}

bool int_xor_trie_count_xor_less_than(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t limit,
    size_t *out_count
) {
    if(trie==NULL||out_count==NULL)return false;

    size_t result=0;
    size_t node=0;

    if(trie->size==0) {
        *out_count=0;
        return true;
    }

    for(int bit=63;bit>=0;--bit) {
        const unsigned q=
            (unsigned)((query>>(unsigned)bit)&UINT64_C(1));
        const unsigned l=
            (unsigned)((limit>>(unsigned)bit)&UINT64_C(1));

        if(l!=0U) {
            const size_t smaller=trie->nodes[node].child[q];

            if(smaller!=SIZE_MAX) {
                if(SIZE_MAX-result<
                   trie->nodes[smaller].subtree_count) {
                    return false;
                }

                result+=trie->nodes[smaller].subtree_count;
            }

            const size_t equal=trie->nodes[node].child[q^1U];

            if(equal==SIZE_MAX||
               trie->nodes[equal].subtree_count==0) {
                *out_count=result;
                return true;
            }

            node=equal;
        } else {
            const size_t equal=trie->nodes[node].child[q];

            if(equal==SIZE_MAX||
               trie->nodes[equal].subtree_count==0) {
                *out_count=result;
                return true;
            }

            node=equal;
        }
    }

    *out_count=result;
    return true;
}

typedef struct {
    bool ok;
    size_t live_count;
    size_t reachable_nodes;
} ValidateResult;

static ValidateResult validate_rec(
    const IntXorTrie *trie,
    size_t node_index,
    unsigned depth,
    bool *seen
) {
    if(node_index>=trie->node_count||seen[node_index]) {
        return (ValidateResult){.ok=false};
    }

    seen[node_index]=true;

    const XorNode *node=&trie->nodes[node_index];
    size_t children_total=0;
    size_t reachable=1;

    for(unsigned b=0;b<2;++b) {
        const size_t child=node->child[b];

        if(child==SIZE_MAX)continue;

        ValidateResult r=validate_rec(
            trie,child,depth+1U,seen
        );

        if(!r.ok)return r;

        if(SIZE_MAX-children_total<r.live_count) {
            return (ValidateResult){.ok=false};
        }

        children_total+=r.live_count;
        reachable+=r.reachable_nodes;
    }

    if(depth<64U&&node->terminal_count!=0) {
        return (ValidateResult){.ok=false};
    }

    if(depth==64U) {
        if(node->child[0]!=SIZE_MAX||
           node->child[1]!=SIZE_MAX||
           node->subtree_count!=node->terminal_count) {
            return (ValidateResult){.ok=false};
        }
    } else if(node->subtree_count!=children_total) {
        return (ValidateResult){.ok=false};
    }

    return (ValidateResult){
        .ok=true,
        .live_count=node->subtree_count,
        .reachable_nodes=reachable
    };
}

bool int_xor_trie_validate(const IntXorTrie *trie) {
    if(trie==NULL||trie->nodes==NULL||
       trie->node_count==0||
       trie->node_count>trie->node_capacity) {
        return false;
    }

    bool *seen=calloc(trie->node_count,sizeof *seen);

    if(seen==NULL)return false;

    ValidateResult result=validate_rec(
        trie,0,0,seen
    );

    const bool all_reachable=result.ok&&
        result.reachable_nodes==trie->node_count;

    free(seen);

    return all_reachable&&
           result.live_count==trie->size;
}
