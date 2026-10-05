#include "int_bstar_tree.h"
#include "int_btree.h"

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
    const size_t orders[] = {6,12,30};
    const int n = 30000;

    puts("order,bstar_height,btree_height,bstar_utilization,bstar_insert_s,btree_insert_s");

    for (size_t i = 0;
         i < sizeof orders / sizeof orders[0];
         ++i) {
        const size_t order = orders[i];

        IntBStarTree *bstar =
            int_bstar_tree_create(order);
        IntBTree *btree =
            int_btree_create(order / 2U);

        if (bstar == NULL || btree == NULL) return 1;

        struct timespec a;
        struct timespec b;

        timespec_get(&a, TIME_UTC);

        for (int key = 0; key < n; ++key) {
            if (!int_bstar_tree_insert(bstar, key)) {
                return 1;
            }
        }

        timespec_get(&b, TIME_UTC);
        const double bstar_time = elapsed(a, b);

        timespec_get(&a, TIME_UTC);

        for (int key = 0; key < n; ++key) {
            if (!int_btree_insert(btree, key)) {
                return 1;
            }
        }

        timespec_get(&b, TIME_UTC);
        const double btree_time = elapsed(a, b);

        size_t hs = 0;
        size_t hb = 0;

        int_bstar_tree_height(bstar, &hs);
        int_btree_height(btree, &hb);

        printf(
            "%zu,%zu,%zu,%.6f,%.9f,%.9f\n",
            order,
            hs,
            hb,
            int_bstar_tree_utilization(bstar),
            bstar_time,
            btree_time
        );

        int_bstar_tree_free(bstar);
        int_btree_free(btree);
    }

    puts("Same nominal maximum fanout does not imply identical constants; B* performs more redistribution work to target higher occupancy.");
    return 0;
}
