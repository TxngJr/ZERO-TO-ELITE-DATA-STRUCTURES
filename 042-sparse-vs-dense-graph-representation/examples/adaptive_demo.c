#include "adaptive_graph.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    AdaptiveGraph *graph=adaptive_graph_create(
        20,false,20,10
    );
    assert(graph!=NULL);

    printf("initial kind=%d density=%.2f%%\n",
           (int)adaptive_graph_backend_kind(graph),
           adaptive_graph_density_percent(graph));

    size_t added=0;

    for(size_t u=0;u<20 && added<45;++u) {
        for(size_t v=u+1;v<20 && added<45;++v) {
            assert(adaptive_graph_add_edge(
                graph,u,v,(int)(u+v)
            ));
            ++added;
        }
    }

    printf(
        "after growth kind=%d density=%.2f%% switches=%zu bytes=%zu\n",
        (int)adaptive_graph_backend_kind(graph),
        adaptive_graph_density_percent(graph),
        adaptive_graph_switch_count(graph),
        adaptive_graph_memory_bytes(graph)
    );

    assert(adaptive_graph_validate(graph));
    adaptive_graph_free(graph);
    return 0;
}
