#include "graph_db.h"
#include <assert.h>
#include <stdio.h>
enum{N=20000,EXTRA=80000};
static uint64_t id_for(size_t i){return UINT64_C(1000000)+i*7U;}
int main(void){
    GraphDb*g=gdb_create(N,110000);assert(g);
    for(size_t i=0;i<N;++i)assert(gdb_add_node(g,id_for(i),(uint32_t)(i%25U),(int64_t)i*3));
    for(size_t i=0;i+1U<N;++i)assert(gdb_add_edge(g,id_for(i),id_for(i+1U),1U,1));
    for(size_t i=0;i<EXTRA;++i){
        size_t s=i%N,d=(i*37U+123U)%N;
        assert(gdb_add_edge(g,id_for(s),id_for(d),2U,(int64_t)i));
    }
    assert(gdb_edge_count(g)==99999U&&gdb_build_indexes(g)&&gdb_validate(g));

    uint64_t ids[1000];size_t count=0U;
    assert(gdb_nodes_with_label(g,7U,ids,1000,&count)&&count==800U);
    for(size_t i=1;i<count;++i)assert(ids[i-1U]<ids[i]);

    assert(gdb_out_neighbors(g,id_for(100),1U,ids,1000,&count)&&count==1U&&ids[0]==id_for(101));
    assert(gdb_in_neighbors(g,id_for(100),1U,ids,1000,&count)&&count==1U&&ids[0]==id_for(99));

    size_t hops=0U;bool found=false;
    assert(gdb_shortest_hops(g,id_for(0),id_for(999),1U,&hops,&found)&&found&&hops==999U);

    assert(gdb_add_edge(g,id_for(5),id_for(10),3U,77));
    assert(!gdb_indexes_fresh(g));
    assert(!gdb_out_neighbors(g,id_for(5),3U,ids,1000,&count));
    assert(gdb_build_indexes(g)&&gdb_validate(g));
    assert(gdb_out_neighbors(g,id_for(5),3U,ids,1000,&count)&&count==1U&&ids[0]==id_for(10));

    uint32_t label=0;int64_t property=0;
    assert(gdb_node(g,id_for(1234),&label,&property)&&label==1234U%25U&&property==1234*3);
    printf("Graph DB tests passed; nodes=%zu edges=%zu\n",gdb_node_count(g),gdb_edge_count(g));
    gdb_free(g);return 0;
}
