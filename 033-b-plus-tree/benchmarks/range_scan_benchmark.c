#include "int_bplus_tree.h"
#include "int_btree.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const int n=100000;
    const int low=40000;
    const int high=59999;
    const size_t expected=(size_t)(high-low+1);

    IntBPlusTree *bplus=int_bplus_tree_create(32);
    IntBTree *btree=int_btree_create(32);

    if(!bplus||!btree)return 1;

    for(int i=0;i<n;++i) {
        if(!int_bplus_tree_insert(bplus,i))return 1;
        if(!int_btree_insert(btree,i))return 1;
    }

    int *range=malloc(expected*sizeof *range);
    int *all=malloc((size_t)n*sizeof *all);
    if(!range||!all)return 1;

    struct timespec a,b;
    size_t written=0;

    timespec_get(&a,TIME_UTC);
    if(!int_bplus_tree_range(
            bplus,low,high,range,expected,&written
        )) return 1;
    timespec_get(&b,TIME_UTC);
    const double bplus_s=elapsed(a,b);

    size_t all_written=0;
    timespec_get(&a,TIME_UTC);
    if(!int_btree_inorder(btree,all,(size_t)n,&all_written))return 1;

    volatile size_t count=0;
    for(size_t i=0;i<all_written;++i) {
        if(all[i]>=low&&all[i]<=high)++count;
    }
    timespec_get(&b,TIME_UTC);
    const double btree_full_scan_s=elapsed(a,b);

    printf(
        "n=%d output=%zu bplus_range=%.9f btree_full_inorder_filter=%.9f\n",
        n,written,bplus_s,btree_full_scan_s
    );

    puts("The B-Tree comparison intentionally uses full inorder filtering because this course API has no cursor/range primitive.");

    (void)count;
    free(range);
    free(all);
    int_bplus_tree_free(bplus);
    int_btree_free(btree);
    return 0;
}
