#include "graph_repr.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    bool seen[64];
    int weight[64];
    size_t count;
} NeighborSet;

static bool collect_neighbor(
    size_t neighbor,
    int weight,
    void *context
) {
    NeighborSet *set=context;

    assert(neighbor<64);
    assert(!set->seen[neighbor]);

    set->seen[neighbor]=true;
    set->weight[neighbor]=weight;
    ++set->count;

    return true;
}

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void assert_same_neighbors(
    GraphRepr **graphs,
    size_t vertex
) {
    NeighborSet sets[3];
    memset(sets,0,sizeof sets);

    for(size_t i=0;i<3;++i) {
        assert(graph_repr_for_each_neighbor(
            graphs[i],vertex,collect_neighbor,&sets[i]
        ));
    }

    for(size_t i=1;i<3;++i) {
        assert(sets[i].count==sets[0].count);

        for(size_t v=0;v<64;++v) {
            assert(sets[i].seen[v]==sets[0].seen[v]);

            if(sets[0].seen[v]) {
                assert(sets[i].weight[v]==sets[0].weight[v]);
            }
        }
    }
}

static void test_cross_representation(bool directed) {
    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    GraphRepr *g[3];

    for(size_t i=0;i<3;++i) {
        g[i]=graph_repr_create(32,directed,kinds[i]);
        assert(g[i]!=NULL);
    }

    bool present[32][32];
    int weights[32][32];

    memset(present,0,sizeof present);
    memset(weights,0,sizeof weights);

    uint32_t rng=directed?0x3900D123u:0x3900A123u;

    for(int step=0;step<12000;++step) {
        size_t u=next_rng(&rng)%32U;
        size_t v=next_rng(&rng)%32U;

        if(u==v) continue;

        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const int weight=(int)next_rng(&rng);
            const bool expected=!present[u][v];

            for(size_t i=0;i<3;++i) {
                assert(graph_repr_add_edge(g[i],u,v,weight)==expected);
            }

            if(expected) {
                present[u][v]=true;
                weights[u][v]=weight;

                if(!directed) {
                    present[v][u]=true;
                    weights[v][u]=weight;
                }
            }
        } else if(op==1U) {
            const bool expected=present[u][v];

            for(size_t i=0;i<3;++i) {
                assert(graph_repr_remove_edge(g[i],u,v)==expected);
            }

            if(expected) {
                present[u][v]=false;

                if(!directed) {
                    present[v][u]=false;
                }
            }
        } else {
            for(size_t i=0;i<3;++i) {
                int weight=0;

                const bool actual=graph_repr_has_edge(
                    g[i],u,v,&weight
                );

                assert(actual==present[u][v]);

                if(actual) {
                    assert(weight==weights[u][v]);
                }
            }
        }

        if((step%211)==0) {
            for(size_t i=0;i<3;++i) {
                assert(graph_repr_validate(g[i]));
            }

            assert(graph_repr_edge_count(g[0])==
                   graph_repr_edge_count(g[1]));
            assert(graph_repr_edge_count(g[0])==
                   graph_repr_edge_count(g[2]));

            assert_same_neighbors(g,next_rng(&rng)%32U);
        }
    }

    for(size_t i=0;i<3;++i) {
        assert(graph_repr_validate(g[i]));
        graph_repr_free(g[i]);
    }
}

int main(void) {
    test_cross_representation(false);
    test_cross_representation(true);

    puts("Graph representation tests passed");
    return 0;
}
