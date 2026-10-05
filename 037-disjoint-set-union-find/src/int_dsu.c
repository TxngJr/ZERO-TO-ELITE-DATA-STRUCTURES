#include "int_dsu.h"

#include <stdint.h>
#include <stdlib.h>

struct IntDSU {
    size_t count;
    size_t components;
    size_t *parent;
    size_t *size;
};

IntDSU *int_dsu_create(size_t count) {
    IntDSU *dsu = calloc(1, sizeof *dsu);
    if (dsu == NULL) return NULL;

    if (count > 0 &&
        (count > SIZE_MAX / sizeof *dsu->parent ||
         count > SIZE_MAX / sizeof *dsu->size)) {
        free(dsu);
        return NULL;
    }

    if (count > 0) {
        dsu->parent = malloc(count * sizeof *dsu->parent);
        dsu->size = malloc(count * sizeof *dsu->size);

        if (dsu->parent == NULL || dsu->size == NULL) {
            free(dsu->parent);
            free(dsu->size);
            free(dsu);
            return NULL;
        }
    }

    dsu->count = count;
    dsu->components = count;

    for (size_t i = 0; i < count; ++i) {
        dsu->parent[i] = i;
        dsu->size[i] = 1;
    }

    return dsu;
}

void int_dsu_free(IntDSU *dsu) {
    if (dsu == NULL) return;
    free(dsu->parent);
    free(dsu->size);
    free(dsu);
}

size_t int_dsu_count(const IntDSU *dsu) {
    return dsu == NULL ? 0 : dsu->count;
}

size_t int_dsu_component_count(const IntDSU *dsu) {
    return dsu == NULL ? 0 : dsu->components;
}

bool int_dsu_find(
    IntDSU *dsu,
    size_t element,
    size_t *out_root
) {
    if (dsu == NULL || out_root == NULL || element >= dsu->count) {
        return false;
    }

    size_t root = element;

    while (dsu->parent[root] != root) {
        root = dsu->parent[root];
    }

    size_t cursor = element;

    while (dsu->parent[cursor] != cursor) {
        const size_t next = dsu->parent[cursor];
        dsu->parent[cursor] = root;
        cursor = next;
    }

    *out_root = root;
    return true;
}

bool int_dsu_connected(
    IntDSU *dsu,
    size_t a,
    size_t b,
    bool *out_connected
) {
    if (out_connected == NULL) return false;

    size_t ra = 0;
    size_t rb = 0;

    if (!int_dsu_find(dsu, a, &ra) ||
        !int_dsu_find(dsu, b, &rb)) {
        return false;
    }

    *out_connected = ra == rb;
    return true;
}

bool int_dsu_union(
    IntDSU *dsu,
    size_t a,
    size_t b,
    bool *out_merged
) {
    if (dsu == NULL || out_merged == NULL) return false;

    size_t ra = 0;
    size_t rb = 0;

    if (!int_dsu_find(dsu, a, &ra) ||
        !int_dsu_find(dsu, b, &rb)) {
        return false;
    }

    if (ra == rb) {
        *out_merged = false;
        return true;
    }

    if (dsu->size[ra] < dsu->size[rb]) {
        const size_t tmp = ra;
        ra = rb;
        rb = tmp;
    }

    dsu->parent[rb] = ra;
    dsu->size[ra] += dsu->size[rb];
    --dsu->components;

    *out_merged = true;
    return true;
}

bool int_dsu_component_size(
    IntDSU *dsu,
    size_t element,
    size_t *out_size
) {
    if (out_size == NULL) return false;

    size_t root = 0;

    if (!int_dsu_find(dsu, element, &root)) {
        return false;
    }

    *out_size = dsu->size[root];
    return true;
}

bool int_dsu_max_depth(
    const IntDSU *dsu,
    size_t *out_depth
) {
    if (dsu == NULL || out_depth == NULL) return false;

    size_t maximum = 0;

    for (size_t i = 0; i < dsu->count; ++i) {
        size_t depth = 0;
        size_t cursor = i;

        while (dsu->parent[cursor] != cursor) {
            cursor = dsu->parent[cursor];
            ++depth;

            if (depth > dsu->count) return false;
        }

        if (depth > maximum) maximum = depth;
    }

    *out_depth = maximum;
    return true;
}

bool int_dsu_validate(const IntDSU *dsu) {
    if (dsu == NULL) return false;

    if (dsu->components > dsu->count) return false;

    size_t roots = 0;
    size_t size_sum = 0;

    for (size_t i = 0; i < dsu->count; ++i) {
        if (dsu->parent[i] >= dsu->count) return false;

        size_t cursor = i;
        size_t steps = 0;

        while (dsu->parent[cursor] != cursor) {
            cursor = dsu->parent[cursor];

            if (++steps > dsu->count) {
                return false;
            }
        }
    }

    for (size_t root = 0; root < dsu->count; ++root) {
        if (dsu->parent[root] != root) continue;

        ++roots;
        size_t actual_size = 0;

        for (size_t i = 0; i < dsu->count; ++i) {
            size_t cursor = i;

            while (dsu->parent[cursor] != cursor) {
                cursor = dsu->parent[cursor];
            }

            if (cursor == root) ++actual_size;
        }

        if (dsu->size[root] != actual_size) return false;

        if (SIZE_MAX - size_sum < actual_size) return false;
        size_sum += actual_size;
    }

    return roots == dsu->components &&
           size_sum == dsu->count;
}
