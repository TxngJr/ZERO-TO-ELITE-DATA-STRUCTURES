#include "int_lsm_tree.h"

#include <stdio.h>
#include <time.h>

static double elapsed(
    struct timespec a,
    struct timespec b
) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) /
               1000000000.0;
}

int main(void) {
    const int runs = 64;
    const int per_run = 128;
    const int probes = 200000;

    IntLSMTree *tree =
        int_lsm_tree_create((size_t)per_run);

    if (tree == NULL) return 1;

    for (int r = 0; r < runs; ++r) {
        for (int i = 0; i < per_run; ++i) {
            const int key =
                r * per_run + i;

            if (!int_lsm_tree_put(
                    tree,
                    key,
                    key * 2
                )) {
                return 1;
            }
        }

        if (!int_lsm_tree_flush(tree)) {
            return 1;
        }
    }

    const size_t before_runs =
        int_lsm_tree_run_count(tree);

    struct timespec a;
    struct timespec b;

    volatile int hits = 0;

    timespec_get(&a, TIME_UTC);

    for (int i = 0; i < probes; ++i) {
        int value = 0;
        hits += int_lsm_tree_get(
            tree,
            -1,
            &value
        ) ? 1 : 0;
    }

    timespec_get(&b, TIME_UTC);
    const double before = elapsed(a, b);

    if (!int_lsm_tree_compact(tree)) return 1;

    const size_t after_runs =
        int_lsm_tree_run_count(tree);

    timespec_get(&a, TIME_UTC);

    for (int i = 0; i < probes; ++i) {
        int value = 0;
        hits += int_lsm_tree_get(
            tree,
            -1,
            &value
        ) ? 1 : 0;
    }

    timespec_get(&b, TIME_UTC);
    const double after = elapsed(a, b);

    printf(
        "runs_before=%zu runs_after=%zu miss_probe_before_s=%.9f miss_probe_after_s=%.9f\n",
        before_runs,
        after_runs,
        before,
        after
    );

    puts("Compaction reduces run probes in this model but incurs rewrite cost; timings are machine-specific.");

    (void)hits;
    int_lsm_tree_free(tree);
    return 0;
}
