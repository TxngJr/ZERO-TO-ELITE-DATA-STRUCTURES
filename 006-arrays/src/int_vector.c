#include "int_vector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct IntVector {
    int *data;
    size_t size;
    size_t capacity;
};

static bool capacity_bytes(size_t capacity, size_t *bytes) {
    if (bytes == NULL) {
        return false;
    }
    if (capacity > SIZE_MAX / sizeof(int)) {
        return false;
    }
    *bytes = capacity * sizeof(int);
    return true;
}

IntVector *int_vector_create(void) {
    return calloc(1, sizeof(IntVector));
}

void int_vector_free(IntVector *vector) {
    if (vector == NULL) {
        return;
    }
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    free(vector);
}

size_t int_vector_size(const IntVector *vector) {
    return vector == NULL ? 0 : vector->size;
}

size_t int_vector_capacity(const IntVector *vector) {
    return vector == NULL ? 0 : vector->capacity;
}

bool int_vector_is_empty(const IntVector *vector) {
    return int_vector_size(vector) == 0;
}

bool int_vector_reserve(IntVector *vector, size_t min_capacity) {
    if (vector == NULL) {
        return false;
    }
    if (min_capacity <= vector->capacity) {
        return true;
    }

    size_t new_capacity = vector->capacity == 0 ? 4 : vector->capacity;

    while (new_capacity < min_capacity) {
        if (new_capacity > SIZE_MAX / 2) {
            new_capacity = min_capacity;
            break;
        }
        new_capacity *= 2;
    }

    size_t bytes = 0;
    if (!capacity_bytes(new_capacity, &bytes)) {
        return false;
    }

    int *new_data = realloc(vector->data, bytes);
    if (new_data == NULL) {
        return false;
    }

    vector->data = new_data;
    vector->capacity = new_capacity;
    return true;
}

bool int_vector_shrink_to_fit(IntVector *vector) {
    if (vector == NULL) {
        return false;
    }
    if (vector->size == vector->capacity) {
        return true;
    }

    if (vector->size == 0) {
        free(vector->data);
        vector->data = NULL;
        vector->capacity = 0;
        return true;
    }

    size_t bytes = 0;
    if (!capacity_bytes(vector->size, &bytes)) {
        return false;
    }

    int *new_data = realloc(vector->data, bytes);
    if (new_data == NULL) {
        return false;
    }

    vector->data = new_data;
    vector->capacity = vector->size;
    return true;
}

bool int_vector_push(IntVector *vector, int value) {
    if (vector == NULL || vector->size == SIZE_MAX) {
        return false;
    }

    if (!int_vector_reserve(vector, vector->size + 1)) {
        return false;
    }

    vector->data[vector->size] = value;
    ++vector->size;
    return true;
}

bool int_vector_pop(IntVector *vector, int *out) {
    if (vector == NULL || vector->size == 0) {
        return false;
    }

    const int value = vector->data[vector->size - 1];
    --vector->size;

    if (out != NULL) {
        *out = value;
    }
    return true;
}

bool int_vector_get(const IntVector *vector, size_t index, int *out) {
    if (vector == NULL || out == NULL || index >= vector->size) {
        return false;
    }
    *out = vector->data[index];
    return true;
}

bool int_vector_set(IntVector *vector, size_t index, int value) {
    if (vector == NULL || index >= vector->size) {
        return false;
    }
    vector->data[index] = value;
    return true;
}

bool int_vector_insert(IntVector *vector, size_t index, int value) {
    if (vector == NULL || index > vector->size || vector->size == SIZE_MAX) {
        return false;
    }

    if (!int_vector_reserve(vector, vector->size + 1)) {
        return false;
    }

    const size_t tail = vector->size - index;
    if (tail > 0) {
        memmove(
            &vector->data[index + 1],
            &vector->data[index],
            tail * sizeof *vector->data
        );
    }

    vector->data[index] = value;
    ++vector->size;
    return true;
}

bool int_vector_erase(IntVector *vector, size_t index, int *out) {
    if (vector == NULL || index >= vector->size) {
        return false;
    }

    const int removed = vector->data[index];
    const size_t tail = vector->size - index - 1;

    if (tail > 0) {
        memmove(
            &vector->data[index],
            &vector->data[index + 1],
            tail * sizeof *vector->data
        );
    }

    --vector->size;
    if (out != NULL) {
        *out = removed;
    }
    return true;
}

void int_vector_clear(IntVector *vector) {
    if (vector != NULL) {
        vector->size = 0;
    }
}

const int *int_vector_data(const IntVector *vector) {
    return vector == NULL ? NULL : vector->data;
}
