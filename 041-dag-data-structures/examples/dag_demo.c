#include "int_dag.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntDAG *dag=int_dag_create(6);
    assert(dag!=NULL);

    assert(int_dag_add_edge(dag,0,2));
    assert(int_dag_add_edge(dag,1,2));
    assert(int_dag_add_edge(dag,2,3));
    assert(int_dag_add_edge(dag,2,4));
    assert(int_dag_add_edge(dag,3,5));
    assert(int_dag_add_edge(dag,4,5));

    size_t order[6];
    size_t written=0;

    assert(int_dag_topological_sort(
        dag,order,6,&written
    ));

    printf("topological order:");
    for(size_t i=0;i<written;++i)printf(" %zu",order[i]);
    putchar('\n');

    assert(!int_dag_add_edge(dag,5,0));
    assert(int_dag_validate(dag));

    int_dag_free(dag);
    return 0;
}
