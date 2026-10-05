#include "graph_traversal.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static void run_case(
    const char *name,
    size_t vertices,
    size_t target_edges
) {
    const GraphReprKind kinds[]={
        GRAPH_REPR_EDGE_LIST,
        GRAPH_REPR_ADJ_MATRIX,
        GRAPH_REPR_ADJ_LIST
    };

    for(size_t k=0;k<3;++k) {
        GraphRepr *g=graph_repr_create(
            vertices,false,kinds[k]
        );
        GraphTraversal *t=graph_traversal_create(vertices);

        if(g==NULL || t==NULL)return;

        for(size_t i=1;i<vertices;++i) {
            (void)graph_repr_add_edge(g,i-1,i,1);
        }

        uint32_t rng=123456789u;

        while(graph_repr_edge_count(g)<target_edges) {
            rng=rng*1664525u+1013904223u;
            const size_t u=rng%vertices;

            rng=rng*1664525u+1013904223u;
            const size_t v=rng%vertices;

            if(u!=v) {
                (void)graph_repr_add_edge(g,u,v,1);
            }
        }

        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(int repeat=0;repeat<5;++repeat) {
            if(!graph_traversal_bfs(
                    t,g,(size_t)repeat%vertices
                )) {
                return;
            }
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "%s kind=%d V=%zu E=%zu bfs5_seconds=%.9f\n",
            name,
            (int)kinds[k],
            vertices,
            graph_repr_edge_count(g),
            elapsed(a,b)
        );

        graph_traversal_free(t);
        graph_repr_free(g);
    }
}

int main(void) {
    run_case("sparse",1000,4000);
    run_case("dense-ish",300,25000);

    puts("Traversal runtime includes representation-specific neighbor enumeration cost.");
    return 0;
}
