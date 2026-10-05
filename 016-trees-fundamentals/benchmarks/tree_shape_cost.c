#include "tree_math.h"

#include <stdio.h>

int main(void) {
    const size_t ns[] = {15, 255, 4095, 65535};

    puts("nodes,min_possible_height,max_possible_height");

    for (size_t i = 0; i < sizeof ns / sizeof ns[0]; ++i) {
        size_t min_h = 0;
        size_t max_h = 0;

        if (!binary_tree_min_height(ns[i], &min_h) ||
            !binary_tree_max_height(ns[i], &max_h)) {
            return 1;
        }

        printf("%zu,%zu,%zu\n", ns[i], min_h, max_h);
    }

    puts("This reports structural bounds, not wall-clock timings.");
    return 0;
}
