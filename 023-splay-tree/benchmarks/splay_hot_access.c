#include "int_bst.h"
#include "int_splay_tree.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const int n = 4000;
    const int hot = n / 3;
    const int repetitions = 200000;

    IntBST *bst = int_bst_create();
    IntSplayTree *splay = int_splay_create();

    if (bst == NULL || splay == NULL) return 1;

    for (int i = 0; i < n; ++i) {
        int key = (i * 1543) % n;

        if (!int_bst_insert(bst, key, NULL)) return 1;
        if (!int_splay_insert(splay, key)) return 1;
    }

    if (!int_splay_contains(splay, hot)) return 1;

    volatile int hits = 0;
    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < repetitions; ++i) {
        hits += int_bst_contains(bst, hot) ? 1 : 0;
    }
    timespec_get(&b, TIME_UTC);
    const double bst_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    for (int i = 0; i < repetitions; ++i) {
        hits += int_splay_contains(splay, hot) ? 1 : 0;
    }
    timespec_get(&b, TIME_UTC);
    const double splay_time = elapsed(a, b);

    printf("repetitions=%d bst_seconds=%.6f splay_seconds=%.6f\n",
           repetitions,
           bst_time,
           splay_time);

    puts("This hot-key workload illustrates locality; it is not a universal performance ranking.");

    (void)hits;
    int_bst_free(bst);
    int_splay_free(splay);
    return 0;
}
