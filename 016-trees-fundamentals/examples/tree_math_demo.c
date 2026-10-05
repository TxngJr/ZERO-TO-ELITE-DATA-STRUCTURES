#include "tree_math.h"

#include <stdio.h>

int main(void) {
    const size_t ns[] = {1, 2, 3, 7, 8, 15, 16};

    puts("n,min_height,max_height");

    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        size_t min_h = 0;
        size_t max_h = 0;

        if (!binary_tree_min_height(ns[i], &min_h) ||
            !binary_tree_max_height(ns[i], &max_h)) {
            return 1;
        }

        printf("%zu,%zu,%zu\n", ns[i], min_h, max_h);
    }

    return 0;
}
