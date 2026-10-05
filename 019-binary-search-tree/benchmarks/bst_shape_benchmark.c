#include "int_bst.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static void shuffle(int *data, size_t n) {
    unsigned x = 123456789u;
    for (size_t i = n; i > 1; --i) {
        x = x * 1664525u + 1013904223u;
        size_t j = x % i;
        int tmp = data[i - 1];
        data[i - 1] = data[j];
        data[j] = tmp;
    }
}

static IntBST *build(const int *keys, size_t n) {
    IntBST *tree = int_bst_create();
    if (tree == NULL) return NULL;

    for (size_t i = 0; i < n; ++i) {
        if (!int_bst_insert(tree, keys[i], NULL)) {
            int_bst_free(tree);
            return NULL;
        }
    }
    return tree;
}

int main(void) {
    const size_t n = 3000;
    int *sorted = malloc(n * sizeof *sorted);
    int *shuffled = malloc(n * sizeof *shuffled);
    if (sorted == NULL || shuffled == NULL) return 1;

    for (size_t i = 0; i < n; ++i) {
        sorted[i] = (int)i;
        shuffled[i] = (int)i;
    }
    shuffle(shuffled, n);

    struct timespec a, b;

    timespec_get(&a, TIME_UTC);
    IntBST *skew = build(sorted, n);
    timespec_get(&b, TIME_UTC);
    double skew_build = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    IntBST *randomish = build(shuffled, n);
    timespec_get(&b, TIME_UTC);
    double random_build = elapsed(a, b);

    if (skew == NULL || randomish == NULL) return 1;

    size_t hs = 0, hr = 0;
    int_bst_height(skew, &hs);
    int_bst_height(randomish, &hr);

    printf("n=%zu sorted_height=%zu shuffled_height=%zu sorted_build=%.6f shuffled_build=%.6f\n",
           n, hs, hr, skew_build, random_build);

    puts("Insertion order controls plain-BST shape; shuffled height is not a deterministic balance guarantee.");

    int_bst_free(skew);
    int_bst_free(randomish);
    free(sorted);
    free(shuffled);
    return 0;
}
