#include "adaptive_graph.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void test_threshold_switching(void) {
    AdaptiveGraph *graph=adaptive_graph_create(
        20,false,20,10
    );
    assert(graph!=NULL);

    size_t us[190];
    size_t vs[190];
    size_t count=0;

    for(size_t u=0;u<20 &&
        adaptive_graph_backend_kind(graph)==
            GRAPH_REPR_ADJ_LIST;++u) {
        for(size_t v=u+1;v<20 &&
            adaptive_graph_backend_kind(graph)==
                GRAPH_REPR_ADJ_LIST;++v) {
            us[count]=u;
            vs[count]=v;

            assert(adaptive_graph_add_edge(
                graph,u,v,(int)(1000+count)
            ));
            ++count;
        }
    }

    assert(adaptive_graph_backend_kind(graph)==
           GRAPH_REPR_ADJ_MATRIX);
    assert(count>=38);
    assert(adaptive_graph_switch_count(graph)==1);

    for(size_t i=0;i<count;++i) {
        int weight=0;
        assert(adaptive_graph_has_edge(
            graph,us[i],vs[i],&weight
        ));
        assert(weight==(int)(1000+i));
    }

    size_t remaining=count;

    for(size_t i=0;i<count &&
        adaptive_graph_backend_kind(graph)==
            GRAPH_REPR_ADJ_MATRIX;++i) {
        assert(adaptive_graph_remove_edge(
            graph,us[i],vs[i]
        ));
        --remaining;
    }

    assert(adaptive_graph_backend_kind(graph)==
           GRAPH_REPR_ADJ_LIST);
    assert(adaptive_graph_switch_count(graph)==2);
    assert(remaining<=19);

    const size_t switches=adaptive_graph_switch_count(graph);

    for(size_t i=0;i<count && remaining<30;++i) {
        if(!adaptive_graph_has_edge(
                graph,us[i],vs[i],NULL
            )) {
            assert(adaptive_graph_add_edge(
                graph,us[i],vs[i],(int)(2000+i)
            ));
            ++remaining;
        }
    }

    assert(remaining<38);
    assert(adaptive_graph_backend_kind(graph)==
           GRAPH_REPR_ADJ_LIST);
    assert(adaptive_graph_switch_count(graph)==switches);
    assert(adaptive_graph_validate(graph));

    adaptive_graph_free(graph);
}

static void test_randomized(bool directed) {
    enum { V=32, STEPS=20000 };

    AdaptiveGraph *graph=adaptive_graph_create(
        V,directed,25,10
    );
    assert(graph!=NULL);

    bool present[V][V];
    int weights[V][V];

    memset(present,0,sizeof present);
    memset(weights,0,sizeof weights);

    uint32_t rng=directed?0x42D12345u:0x42A12345u;

    for(int step=0;step<STEPS;++step) {
        const size_t u=next_rng(&rng)%V;
        const size_t v=next_rng(&rng)%V;
        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const int weight=(int)next_rng(&rng);
            const bool expected=
                u!=v && !present[u][v];

            const bool actual=adaptive_graph_add_edge(
                graph,u,v,weight
            );

            assert(actual==expected);

            if(actual) {
                present[u][v]=true;
                weights[u][v]=weight;

                if(!directed) {
                    present[v][u]=true;
                    weights[v][u]=weight;
                }
            }
        } else if(op==1U) {
            const bool expected=
                u!=v && present[u][v];

            const bool actual=adaptive_graph_remove_edge(
                graph,u,v
            );

            assert(actual==expected);

            if(actual) {
                present[u][v]=false;

                if(!directed) {
                    present[v][u]=false;
                }
            }
        } else {
            int weight=0;

            const bool actual=adaptive_graph_has_edge(
                graph,u,v,&weight
            );

            assert(actual==present[u][v]);

            if(actual) {
                assert(weight==weights[u][v]);
            }
        }

        if((step%197)==0) {
            assert(adaptive_graph_validate(graph));

            for(size_t a=0;a<V;++a) {
                for(size_t b=0;b<V;++b) {
                    int weight=0;
                    const bool actual=
                        adaptive_graph_has_edge(
                            graph,a,b,&weight
                        );

                    assert(actual==present[a][b]);

                    if(actual) {
                        assert(weight==weights[a][b]);
                    }
                }
            }
        }
    }

    assert(adaptive_graph_validate(graph));
    adaptive_graph_free(graph);
}

int main(void) {
    assert(adaptive_graph_create(10,false,20,20)==NULL);
    assert(adaptive_graph_create(10,false,0,0)==NULL);
    assert(adaptive_graph_create(10,false,101,10)==NULL);

    test_threshold_switching();
    test_randomized(false);
    test_randomized(true);

    puts("Adaptive Graph tests passed");
    return 0;
}
