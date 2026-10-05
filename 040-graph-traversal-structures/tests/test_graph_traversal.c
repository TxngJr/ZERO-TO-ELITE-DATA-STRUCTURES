#include "graph_traversal.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t random_vertex(
    uint32_t *state,
    size_t vertex_count
) {
    /*
     * Use upper LCG bits for bounded sampling.
     * Consecutive low bits of this LCG are strongly correlated;
     * with %96 they previously produced only a small repeating
     * subset of directed pairs and the edge-generation loop
     * could never reach TARGET_EDGES.
     */
    return (size_t)(next_rng(state) >> 8) % vertex_count;
}

static GraphRepr *build_graph(
    GraphReprKind kind,
    bool directed
) {
    GraphRepr *g=graph_repr_create(10,directed,kind);
    assert(g!=NULL);

    const size_t edges[][2]={
        {0,1},{0,2},{1,3},{2,3},{2,4},
        {3,5},{4,5},{5,6},{7,8}
    };

    for(size_t i=0;i<9;++i) {
        assert(graph_repr_add_edge(
            g,edges[i][0],edges[i][1],1
        ));
    }

    return g;
}

static void test_bfs_distances(void) {
    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    const size_t expected_depth[]={
        0,1,1,2,2,3,4,SIZE_MAX,SIZE_MAX,SIZE_MAX
    };

    for(size_t k=0;k<3;++k) {
        GraphRepr *g=build_graph(kinds[k],false);
        GraphTraversal *t=graph_traversal_create(10);

        assert(t!=NULL);
        assert(graph_traversal_bfs(t,g,0));
        assert(graph_traversal_validate(t,g));

        for(size_t v=0;v<10;++v) {
            bool visited=false;
            assert(graph_traversal_visited(t,v,&visited));

            if(expected_depth[v]==SIZE_MAX) {
                assert(!visited);
            } else {
                size_t depth=0;
                assert(visited);
                assert(graph_traversal_depth(t,v,&depth));
                assert(depth==expected_depth[v]);
            }
        }

        size_t path[10];
        size_t written=0;

        assert(graph_traversal_path_to(
            t,6,path,10,&written
        ));
        assert(written==5);
        assert(path[0]==0);
        assert(path[written-1]==6);

        graph_traversal_free(t);
        graph_repr_free(g);
    }
}

static void test_dfs_reachability(void) {
    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    for(size_t k=0;k<3;++k) {
        GraphRepr *g=build_graph(kinds[k],true);
        GraphTraversal *t=graph_traversal_create(10);

        assert(t!=NULL);
        assert(graph_traversal_dfs(t,g,0));
        assert(graph_traversal_validate(t,g));

        for(size_t v=0;v<=6;++v) {
            bool visited=false;
            assert(graph_traversal_visited(t,v,&visited));
            assert(visited);
        }

        for(size_t v=7;v<10;++v) {
            bool visited=false;
            assert(graph_traversal_visited(t,v,&visited));
            assert(!visited);
        }

        graph_traversal_free(t);
        graph_repr_free(g);
    }
}

static void test_random_cross_representation(void) {
    enum { V=96, TARGET_EDGES=700, ROUNDS=60 };

    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    GraphRepr *graphs[3];
    GraphTraversal *trav[3];

    for(size_t k=0;k<3;++k) {
        graphs[k]=graph_repr_create(V,true,kinds[k]);
        trav[k]=graph_traversal_create(V);

        assert(graphs[k]!=NULL);
        assert(trav[k]!=NULL);
    }

    uint32_t rng=0x40A55123u;

    size_t attempts=0;

    while(graph_repr_edge_count(graphs[0])<TARGET_EDGES) {
        assert(attempts++ < TARGET_EDGES * 1000U);

        const size_t u=random_vertex(&rng,V);
        const size_t v=random_vertex(&rng,V);

        if(u==v)continue;

        const bool inserted=
            graph_repr_add_edge(graphs[0],u,v,1);

        if(inserted) {
            assert(graph_repr_add_edge(graphs[1],u,v,1));
            assert(graph_repr_add_edge(graphs[2],u,v,1));
        }
    }

    for(int round=0;round<ROUNDS;++round) {
        const size_t source=random_vertex(&rng,V);

        for(size_t k=0;k<3;++k) {
            assert(graph_traversal_bfs(
                trav[k],graphs[k],source
            ));
            assert(graph_traversal_validate(
                trav[k],graphs[k]
            ));
        }

        for(size_t v=0;v<V;++v) {
            bool visited[3];

            for(size_t k=0;k<3;++k) {
                assert(graph_traversal_visited(
                    trav[k],v,&visited[k]
                ));
            }

            assert(visited[0]==visited[1]);
            assert(visited[0]==visited[2]);

            if(visited[0]) {
                size_t depth[3];

                for(size_t k=0;k<3;++k) {
                    assert(graph_traversal_depth(
                        trav[k],v,&depth[k]
                    ));
                }

                assert(depth[0]==depth[1]);
                assert(depth[0]==depth[2]);
            }
        }

        for(size_t k=0;k<3;++k) {
            assert(graph_traversal_dfs(
                trav[k],graphs[k],source
            ));
            assert(graph_traversal_validate(
                trav[k],graphs[k]
            ));
        }

        for(size_t v=0;v<V;++v) {
            bool visited[3];

            for(size_t k=0;k<3;++k) {
                assert(graph_traversal_visited(
                    trav[k],v,&visited[k]
                ));
            }

            assert(visited[0]==visited[1]);
            assert(visited[0]==visited[2]);
        }
    }

    for(size_t k=0;k<3;++k) {
        graph_traversal_free(trav[k]);
        graph_repr_free(graphs[k]);
    }
}

int main(void) {
    test_bfs_distances();
    test_dfs_reachability();
    test_random_cross_representation();

    puts("Graph traversal tests passed");
    return 0;
}
