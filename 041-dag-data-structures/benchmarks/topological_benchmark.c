#include "int_dag.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static void run_case(size_t n,size_t edges) {
    IntDAG *dag=int_dag_create(n);
    if(dag==NULL)return;

    uint32_t rng=123456789u;

    while(int_dag_edge_count(dag)<edges) {
        rng=rng*1664525u+1013904223u;
        size_t u=rng%n;

        rng=rng*1664525u+1013904223u;
        size_t v=rng%n;

        if(u==v)continue;

        if(u>v) {
            const size_t tmp=u;
            u=v;
            v=tmp;
        }

        (void)int_dag_add_edge(dag,u,v);
    }

    size_t *order=malloc(n*sizeof *order);
    if(order==NULL)return;

    struct timespec a,b;
    size_t written=0;

    timespec_get(&a,TIME_UTC);

    for(int repeat=0;repeat<20;++repeat) {
        if(!int_dag_topological_sort(
                dag,order,n,&written
            )) {
            return;
        }
    }

    timespec_get(&b,TIME_UTC);

    printf(
        "V=%zu E=%zu topo20_seconds=%.9f\n",
        n,
        int_dag_edge_count(dag),
        elapsed(a,b)
    );

    free(order);
    int_dag_free(dag);
}

int main(void) {
    run_case(1000,4000);
    run_case(5000,20000);
    run_case(10000,40000);

    puts("Benchmark measures Kahn topological sorting; dynamic insertion has additional cycle-check cost.");
    return 0;
}
