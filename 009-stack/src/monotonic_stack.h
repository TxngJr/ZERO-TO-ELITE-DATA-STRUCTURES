#ifndef MONOTONIC_STACK_H
#define MONOTONIC_STACK_H

#include <stdbool.h>
#include <stddef.h>

bool next_greater_values(
    const int *input,
    size_t n,
    int unresolved_value,
    int *output
);

#endif
