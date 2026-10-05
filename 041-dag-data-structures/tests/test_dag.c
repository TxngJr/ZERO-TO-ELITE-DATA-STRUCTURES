#include "int_dag.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static bool ref_path(
    bool matrix[48][48],
    size_t source,
    size_t target
) {
    bool visited[48]={0};
    size_t stack[48];
    size_t top=0;

    stack[top++]=source;
    visited[source]=true;

    while(top>0) {
        const size_t u=stack[--top];

        if(u==target)return true;

        for(size_t v=0;v<48;++v) {
            if(matrix[u][v]&&!visited[v]) {
                visited[v]=true;
                stack[top++]=v;
            }
        }
    }

    return false;
}

static void assert_topological(
    const IntDAG *dag,
    bool matrix[48][48],
    size_t n
) {
    size_t order[48];
    size_t written=0;
    size_t pos[48];

    assert(int_dag_topological_sort(
        dag,order,48,&written
    ));
    assert(written==n);

    for(size_t i=0;i<n;++i)pos[order[i]]=i;

    for(size_t u=0;u<n;++u) {
        for(size_t v=0;v<n;++v) {
            if(matrix[u][v]) {
                assert(pos[u]<pos[v]);
            }
        }
    }
}

static void test_basic(void) {
    IntDAG *dag=int_dag_create(7);
    assert(dag!=NULL);

    assert(int_dag_add_edge(dag,0,2));
    assert(int_dag_add_edge(dag,1,2));
    assert(int_dag_add_edge(dag,2,3));
    assert(int_dag_add_edge(dag,2,4));
    assert(int_dag_add_edge(dag,3,5));
    assert(int_dag_add_edge(dag,4,5));

    size_t indegree=0;
    size_t outdegree=0;

    assert(int_dag_in_degree(dag,2,&indegree));
    assert(indegree==2);

    assert(int_dag_out_degree(dag,2,&outdegree));
    assert(outdegree==2);

    bool source=false;
    bool sink=false;

    assert(int_dag_is_source(dag,0,&source)&&source);
    assert(int_dag_is_sink(dag,5,&sink)&&sink);

    assert(!int_dag_add_edge(dag,5,0));
    assert(!int_dag_add_edge(dag,2,2));
    assert(!int_dag_add_edge(dag,0,2));

    assert(int_dag_remove_edge(dag,1,2));
    assert(int_dag_in_degree(dag,2,&indegree));
    assert(indegree==1);

    assert(int_dag_validate(dag));
    int_dag_free(dag);
}

static void test_randomized(void) {
    enum { N=48, STEPS=15000 };

    IntDAG *dag=int_dag_create(N);
    assert(dag!=NULL);

    bool matrix[48][48];
    memset(matrix,0,sizeof matrix);

    uint32_t rng=0x41DA6123u;

    for(int step=0;step<STEPS;++step) {
        const size_t u=next_rng(&rng)%N;
        const size_t v=next_rng(&rng)%N;
        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            bool expected=false;

            if(u!=v&&!matrix[u][v]) {
                expected=!ref_path(matrix,v,u);
            }

            const bool actual=int_dag_add_edge(dag,u,v);
            assert(actual==expected);

            if(actual)matrix[u][v]=true;
        } else if(op==1U) {
            const bool expected=matrix[u][v];
            const bool actual=int_dag_remove_edge(dag,u,v);

            assert(actual==expected);

            if(actual)matrix[u][v]=false;
        } else {
            assert(int_dag_has_edge(dag,u,v)==matrix[u][v]);
        }

        if((step%173)==0) {
            assert(int_dag_validate(dag));
            assert_topological(dag,matrix,N);

            for(size_t vtx=0;vtx<N;++vtx) {
                size_t expected_in=0;
                size_t expected_out=0;

                for(size_t x=0;x<N;++x) {
                    if(matrix[x][vtx])++expected_in;
                    if(matrix[vtx][x])++expected_out;
                }

                size_t actual=0;

                assert(int_dag_in_degree(
                    dag,vtx,&actual
                ));
                assert(actual==expected_in);

                assert(int_dag_out_degree(
                    dag,vtx,&actual
                ));
                assert(actual==expected_out);
            }
        }
    }

    assert(int_dag_validate(dag));
    assert_topological(dag,matrix,N);

    int_dag_free(dag);
}

int main(void) {
    test_basic();
    test_randomized();

    puts("DAG tests passed");
    return 0;
}
