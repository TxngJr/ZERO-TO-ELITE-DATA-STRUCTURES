#include "int_avl.h"
#include "int_bst.h"

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

    if (bst == NULL || avl == NULL) return 1;

    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_bst_insert(bst, i, NULL)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double bst_insert = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < n; ++i) {
        if (!int_avl_insert(avl, i)) return 1;
    }
    timespec_get(&b, TIME_UTC);
    const double avl_insert = elapsed(a, b);

    size_t bst_height = 0;
    size_t avl_height = 0;

    int_bst_height(bst, &bst_height);
    int_avl_height(avl, &avl_height);

    printf(
        "n=%d bst_height=%zu avl_height=%zu bst_insert=%.6f avl_insert=%.6f\n",
        n,
        bst_height,
        avl_height,
        bst_insert,
        avl_insert
    );

    puts("Sorted input is intentionally adversarial for the plain BST. Timings are machine-dependent.");

    int_bst_free(bst);
    int_avl_free(avl);
    return 0;
}
