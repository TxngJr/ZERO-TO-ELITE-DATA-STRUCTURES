#include "graph_db.h"
#include <stdio.h>
int main(void){GraphDb*g=gdb_create(8,16);if(!g)return 1;gdb_add_node(g,10,1,100);gdb_add_node(g,20,1,200);gdb_add_node(g,30,2,300);gdb_add_edge(g,10,20,7,1);gdb_add_edge(g,20,30,7,1);if(!gdb_build_indexes(g))return 2;size_t hops=0;bool found=false;if(!gdb_shortest_hops(g,10,30,7,&hops,&found))return 3;printf("found=%d hops=%zu\n",found,hops);gdb_free(g);return 0;}
