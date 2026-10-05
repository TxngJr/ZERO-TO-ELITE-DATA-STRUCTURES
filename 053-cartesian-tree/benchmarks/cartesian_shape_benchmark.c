#include "int_cartesian_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

static int run_case(
    const char *name,
    int64_t *values,
    size_t n
) {
    struct timespec a,b;

    timespec_get(&a,TIME_UTC);
    IntCartesianTree *tree=
        int_cartesian_tree_create(values,n);
    timespec_get(&b,TIME_UTC);

    if(tree==NULL)return 1;

    size_t height=0;

    if(!int_cartesian_tree_height(tree,&height)) {
        return 1;
    }

    printf(
        "%s,n=%zu,height=%zu,build_seconds=%.9f\n",
        name,n,height,elapsed(a,b)
    );

    int_cartesian_tree_free(tree);
    return 0;
}

int main(void) {
    const size_t n=100000;

    int64_t *values=malloc(n*sizeof *values);
    if(values==NULL)return 1;

    for(size_t i=0;i<n;++i)values[i]=(int64_t)i;

    if(run_case("ascending",values,n)!=0)return 1;

    uint32_t rng=123456789u;

    for(size_t i=0;i<n;++i) {
        rng=rng*1664525u+1013904223u;
        values[i]=(int64_t)rng;
    }

    if(run_case("pseudo_random",values,n)!=0)return 1;

    free(values);
    return 0;
}
