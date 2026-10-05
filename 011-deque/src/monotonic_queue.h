#ifndef MONOTONIC_QUEUE_H
#define MONOTONIC_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

bool sliding_window_maximum(
    const int *input,
    size_t n,
    size_t window,
    int *output
);

#endif
