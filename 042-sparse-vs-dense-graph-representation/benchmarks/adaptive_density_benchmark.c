#include "adaptive_graph.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static double lookup_phase(
    AdaptiveGraph *graph,
    size_t vertices,
    uint32_t *rng
) {
    struct timespec a,b;
    volatile size_t hits=0;

    timespec_get(&a,TIME_UTC);

    for(size_t i=0;i<100000;++i) {
        *rng=*rng*1664525u+1013904223u;
        const size_t u=*rng%vertices;

        *rng=*rng*1664525u+1013904223u;
        const size_t v=*rng%vertices;

        if(adaptive_graph_has_edge(
                graph,u,v,NULL
            )) {
            ++hits;
        }
    }

    timespec_get(&b,TIME_UTC);
    (void)hits;

    return elapsed(a,b);
}

int main(void) {
    const size_t v=300;
    const size_t max_edges=v*(v-1)/2;
    const size_t dense_target=max_edges/4;
    const size_t sparse_target=max_edges/20;

    AdaptiveGraph *graph=adaptive_graph_create(
        v,false,20,8
    );
    if(graph==NULL)return 1;

    size_t *us=malloc(dense_target*sizeof *us);
    size_t *vs=malloc(dense_target*sizeof *vs);

    if(us==NULL||vs==NULL)return 1;

    size_t count=0;

    for(size_t u=0;u<v&&count<dense_target;++u) {
        for(size_t w=u+1;w<v&&count<dense_target;++w) {
            us[count]=u;
            vs[count]=w;

            if(!adaptive_graph_add_edge(
                    graph,u,w,1
                )) {
                return 1;
            }

            ++count;
        }
    }

    uint32_t rng=123456789u;

    const double dense_lookup=
        lookup_phase(graph,v,&rng);

    printf(
        "dense phase kind=%d density=%.2f switches=%zu bytes=%zu lookup_seconds=%.9f\n",
        (int)adaptive_graph_backend_kind(graph),
        adaptive_graph_density_percent(graph),
        adaptive_graph_switch_count(graph),
        adaptive_graph_memory_bytes(graph),
        dense_lookup
    );

    while(count>sparse_target) {
        --count;

        if(!adaptive_graph_remove_edge(
                graph,us[count],vs[count]
            )) {
            return 1;
        }
    }

    const double sparse_lookup=
        lookup_phase(graph,v,&rng);

    printf(
        "sparse phase kind=%d density=%.2f switches=%zu bytes=%zu lookup_seconds=%.9f\n",
        (int)adaptive_graph_backend_kind(graph),
        adaptive_graph_density_percent(graph),
        adaptive_graph_switch_count(graph),
        adaptive_graph_memory_bytes(graph),
        sparse_lookup
    );

    puts("Thresholds are teaching heuristics; optimal switching depends on workload and conversion cost.");

    free(us);
    free(vs);
    adaptive_graph_free(graph);
    return 0;
}
