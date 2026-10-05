#include "int_bst.h"
#include "int_treap.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const int n = 5000;

    IntBST *bst = int_bst_create();
    IntTreap *treap = int_treap_create();

    if (bst == NULL || treap == NULL) return 1;

    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_bst_insert(bst, i, NULL)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double bst_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_treap_insert(treap, i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double treap_time = elapsed(a, b);

    size_t bst_height = 0;
    size_t treap_height = 0;

    int_bst_height(bst, &bst_height);
    int_treap_height(treap, &treap_height);

    printf(
        "n=%d bst_height=%zu treap_height=%zu bst_insert=%.6f treap_insert=%.6f\n",
        n,
        bst_height,
        treap_height,
        bst_time,
        treap_time
    );

    puts("Treap result uses deterministic pseudo-random priorities; expected bounds are distributional, not worst-case.");

    int_bst_free(bst);
    int_treap_free(treap);
    return 0;
}
