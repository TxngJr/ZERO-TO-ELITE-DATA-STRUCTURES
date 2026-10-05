#include "int_avl.h"
#include "int_bst.h"
#include "int_red_black_tree.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const int n = 5000;

    IntBST *bst = int_bst_create();
    IntAVL *avl = int_avl_create();
    IntRedBlackTree *rbt = int_rbt_create();

    if (bst == NULL || avl == NULL || rbt == NULL) return 1;

    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_bst_insert(bst, i, NULL)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double bst_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_avl_insert(avl, i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double avl_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_rbt_insert(rbt, i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double rbt_time = elapsed(a, b);

    size_t hb = 0;
    size_t ha = 0;
    size_t hr = 0;

    int_bst_height(bst, &hb);
    int_avl_height(avl, &ha);
    int_rbt_height(rbt, &hr);

    printf(
        "n=%d bst_h=%zu avl_h=%zu rbt_h=%zu bst_s=%.6f avl_s=%.6f rbt_s=%.6f\n",
        n,
        hb,
        ha,
        hr,
        bst_time,
        avl_time,
        rbt_time
    );

    puts("Sorted input is adversarial for plain BST; timing ratios are machine-specific.");

    int_bst_free(bst);
    int_avl_free(avl);
    int_rbt_free(rbt);
    return 0;
}
