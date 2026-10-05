#include "graph_repr.h"

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
        GraphRepr *g=graph_repr_create(vertices,false,kinds[k]);

        if(g==NULL) {
            printf("%s kind=%d unavailable\n",
                   name,(int)kinds[k]);
            continue;
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
        volatile size_t hits=0;

        timespec_get(&a,TIME_UTC);

        for(size_t i=0;i<50000;++i) {
            rng=rng*1664525u+1013904223u;
            const size_t u=rng%vertices;

            rng=rng*1664525u+1013904223u;
            const size_t v=rng%vertices;

            if(graph_repr_has_edge(g,u,v,NULL))++hits;
        }

        timespec_get(&b,TIME_UTC);

        printf(
            "%s kind=%d V=%zu E=%zu bytes=%zu lookup_seconds=%.9f\n",
            name,
            (int)kinds[k],
            vertices,
            graph_repr_edge_count(g),
            graph_repr_memory_bytes(g),
            elapsed(a,b)
        );

        (void)hits;
        graph_repr_free(g);
    }
}

int main(void) {
    run_case("sparse",1200,4000);
    run_case("dense-ish",400,30000);

    puts("Results depend on density, allocator, cache and operation mix.");
    return 0;
}
