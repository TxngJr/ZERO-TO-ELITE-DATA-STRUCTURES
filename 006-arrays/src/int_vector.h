#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntVector IntVector;

IntVector *int_vector_create(void);
void int_vector_free(IntVector *vector);

size_t int_vector_size(const IntVector *vector);
size_t int_vector_capacity(const IntVector *vector);
bool int_vector_is_empty(const IntVector *vector);

bool int_vector_reserve(IntVector *vector, size_t min_capacity);
bool int_vector_shrink_to_fit(IntVector *vector);

bool int_vector_push(IntVector *vector, int value);
bool int_vector_pop(IntVector *vector, int *out);

bool int_vector_get(const IntVector *vector, size_t index, int *out);
bool int_vector_set(IntVector *vector, size_t index, int value);

bool int_vector_insert(IntVector *vector, size_t index, int value);
bool int_vector_erase(IntVector *vector, size_t index, int *out);

void int_vector_clear(IntVector *vector);

/*
 * Returns a read-only view of the current backing storage.
 * Any mutating operation that reallocates may invalidate this pointer.
 */
const int *int_vector_data(const IntVector *vector);

#endif
