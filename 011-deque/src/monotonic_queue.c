#include "monotonic_queue.h"

#include <stdint.h>
#include <stdlib.h>

bool sliding_window_maximum(
    const int *input,
    size_t n,
    size_t window,
    int *output
) {
    if (window == 0 || window > n) return false;
    if ((input == NULL || output == NULL) && n != 0) return false;
    if (n > SIZE_MAX / sizeof(size_t)) return false;

    size_t *deque = malloc(n * sizeof *deque);
    if (deque == NULL) return false;

    size_t front = 0;
    size_t back = 0;
    size_t out_index = 0;

    for (size_t i = 0; i < n; ++i) {
        while (front < back && deque[front] + window <= i) {
            ++front;
        }

        while (front < back && input[i] >= input[deque[back - 1]]) {
            --back;
        }

        deque[back++] = i;

        if (i + 1 >= window) {
            output[out_index++] = input[deque[front]];
        }
    }

    free(deque);
    return true;
}
