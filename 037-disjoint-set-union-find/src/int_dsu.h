#ifndef INT_DSU_H
#define INT_DSU_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntDSU IntDSU;

IntDSU *int_dsu_create(size_t count);
void int_dsu_free(IntDSU *dsu);

size_t int_dsu_count(const IntDSU *dsu);
size_t int_dsu_component_count(const IntDSU *dsu);

bool int_dsu_find(IntDSU *dsu, size_t element, size_t *out_root);
bool int_dsu_connected(IntDSU *dsu, size_t a, size_t b, bool *out_connected);
bool int_dsu_union(IntDSU *dsu, size_t a, size_t b, bool *out_merged);
bool int_dsu_component_size(IntDSU *dsu, size_t element, size_t *out_size);

bool int_dsu_max_depth(const IntDSU *dsu, size_t *out_depth);
bool int_dsu_validate(const IntDSU *dsu);

#endif
