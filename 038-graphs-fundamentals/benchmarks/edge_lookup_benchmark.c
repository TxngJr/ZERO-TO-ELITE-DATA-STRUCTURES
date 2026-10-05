#include "int_graph.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec) +
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t vertices = 5000;
    const size_t targets[] = {1000,5000,10000,20000};

    puts("edges,lookup_seconds");

    for (size_t t = 0; t < 4; ++t) {
        IntGraph *g = int_graph_create(vertices,true);
        if (g == NULL) return 1;

        uint32_t rng = 123456789u;

        while (int_graph_edge_count(g) < targets[t]) {
            rng = rng * 1664525u + 1013904223u;
            const size_t u = rng % vertices;
            rng = rng * 1664525u + 1013904223u;
            const size_t v = rng % vertices;

            if (u != v) {
                (void)int_graph_add_edge(g,u,v,1);
            }
        }

        struct timespec a,b;
        volatile size_t hits = 0;

        timespec_get(&a,TIME_UTC);

        for (size_t i=0;i<20000;++i) {
            rng = rng * 1664525u + 1013904223u;
            const size_t u = rng % vertices;
            rng = rng * 1664525u + 1013904223u;
            const size_t v = rng % vertices;

            if (int_graph_has_edge(g,u,v,NULL)) ++hits;
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%.9f\n",
               int_graph_edge_count(g),
               elapsed(a,b));

        (void)hits;
        int_graph_free(g);
    }

    puts("Edge-list lookup scales with E; Chapter 039 compares list/matrix/adjacency-list representations.");
    return 0;
}
