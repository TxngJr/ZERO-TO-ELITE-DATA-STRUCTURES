#include "monotonic_stack.h"

#include <stdint.h>
#include <stdlib.h>

bool next_greater_values(
    const int *input,
    size_t n,
    int unresolved_value,
    int *output
) {
    if ((input == NULL || output == NULL) && n != 0) return false;
    if (n == 0) return true;
    if (n > SIZE_MAX / sizeof(size_t)) return false;

    size_t *stack = malloc(n * sizeof *stack);
    if (stack == NULL) return false;
    size_t top = 0;

    for (size_t i = 0; i < n; ++i) output[i] = unresolved_value;

    for (size_t i = 0; i < n; ++i) {
        while (top > 0 && input[i] > input[stack[top - 1]]) {
            const size_t index = stack[--top];
            output[index] = input[i];
        }
        stack[top++] = i;
    }

    free(stack);
    return true;
}
