#include "int_segment_tree.h"

#include <stdint.h>
#include <stdlib.h>

struct IntSegmentTree {
    size_t count;
    size_t capacity;
    int64_t *sum;
    int64_t *min;
};

static int64_t min_i64(int64_t a, int64_t b) {
    return a < b ? a : b;
}

static void build(
    IntSegmentTree *tree,
    const int64_t *values,
    size_t node,
    size_t left,
    size_t right
) {
    if (right - left == 1) {
        tree->sum[node] = values[left];
        tree->min[node] = values[left];
        return;
    }

    const size_t mid = left + (right - left) / 2;
    const size_t lc = node * 2;
    const size_t rc = lc + 1;

    build(tree, values, lc, left, mid);
    build(tree, values, rc, mid, right);

    tree->sum[node] = tree->sum[lc] + tree->sum[rc];
    tree->min[node] = min_i64(tree->min[lc], tree->min[rc]);
}

IntSegmentTree *int_segment_tree_create(
    const int64_t *values,
    size_t count
) {
    if (count > 0 && values == NULL) return NULL;

    IntSegmentTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->count = count;

    if (count == 0) return tree;

    if (count > (SIZE_MAX - 8U) / 4U) {
        free(tree);
        return NULL;
    }

    tree->capacity = count * 4U + 8U;

    if (tree->capacity > SIZE_MAX / sizeof *tree->sum ||
        tree->capacity > SIZE_MAX / sizeof *tree->min) {
        free(tree);
        return NULL;
    }

    tree->sum = calloc(tree->capacity, sizeof *tree->sum);
    tree->min = calloc(tree->capacity, sizeof *tree->min);

    if (tree->sum == NULL || tree->min == NULL) {
        int_segment_tree_free(tree);
        return NULL;
    }

    build(tree, values, 1, 0, count);
    return tree;
}

void int_segment_tree_free(IntSegmentTree *tree) {
    if (tree == NULL) return;
    free(tree->sum);
    free(tree->min);
    free(tree);
}

size_t int_segment_tree_size(const IntSegmentTree *tree) {
    return tree == NULL ? 0 : tree->count;
}

static void point_set_rec(
    IntSegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    size_t index,
    int64_t value
) {
    if (right - left == 1) {
        tree->sum[node] = value;
        tree->min[node] = value;
        return;
    }

    const size_t mid = left + (right - left) / 2;
    const size_t lc = node * 2;
    const size_t rc = lc + 1;

    if (index < mid) {
        point_set_rec(tree, lc, left, mid, index, value);
    } else {
        point_set_rec(tree, rc, mid, right, index, value);
    }

    tree->sum[node] = tree->sum[lc] + tree->sum[rc];
    tree->min[node] = min_i64(tree->min[lc], tree->min[rc]);
}

bool int_segment_tree_point_set(
    IntSegmentTree *tree,
    size_t index,
    int64_t value
) {
    if (tree == NULL || index >= tree->count) return false;

    point_set_rec(tree, 1, 0, tree->count, index, value);
    return true;
}

typedef struct {
    int64_t sum;
    int64_t min;
    bool has_value;
} QueryResult;

static QueryResult query_rec(
    const IntSegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    size_t ql,
    size_t qr
) {
    if (qr <= left || right <= ql) {
        return (QueryResult){0};
    }

    if (ql <= left && right <= qr) {
        return (QueryResult){
            .sum = tree->sum[node],
            .min = tree->min[node],
            .has_value = true
        };
    }

    const size_t mid = left + (right - left) / 2;

    QueryResult a = query_rec(
        tree, node * 2, left, mid, ql, qr
    );
    QueryResult b = query_rec(
        tree, node * 2 + 1, mid, right, ql, qr
    );

    if (!a.has_value) return b;
    if (!b.has_value) return a;

    return (QueryResult){
        .sum = a.sum + b.sum,
        .min = min_i64(a.min, b.min),
        .has_value = true
    };
}

bool int_segment_tree_point_get(
    const IntSegmentTree *tree,
    size_t index,
    int64_t *out_value
) {
    if (tree == NULL || out_value == NULL ||
        index >= tree->count) {
        return false;
    }

    return int_segment_tree_range_sum(
        tree,
        index,
        index + 1,
        out_value
    );
}

bool int_segment_tree_range_sum(
    const IntSegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_sum
) {
    if (tree == NULL || out_sum == NULL ||
        left >= right || right > tree->count) {
        return false;
    }

    const QueryResult result = query_rec(
        tree, 1, 0, tree->count, left, right
    );

    if (!result.has_value) return false;

    *out_sum = result.sum;
    return true;
}

bool int_segment_tree_range_min(
    const IntSegmentTree *tree,
    size_t left,
    size_t right,
    int64_t *out_min
) {
    if (tree == NULL || out_min == NULL ||
        left >= right || right > tree->count) {
        return false;
    }

    const QueryResult result = query_rec(
        tree, 1, 0, tree->count, left, right
    );

    if (!result.has_value) return false;

    *out_min = result.min;
    return true;
}

static bool validate_rec(
    const IntSegmentTree *tree,
    size_t node,
    size_t left,
    size_t right,
    int64_t *out_sum,
    int64_t *out_min
) {
    if (node >= tree->capacity) return false;

    if (right - left == 1) {
        if (tree->sum[node] != tree->min[node]) return false;

        *out_sum = tree->sum[node];
        *out_min = tree->min[node];
        return true;
    }

    const size_t mid = left + (right - left) / 2;

    int64_t left_sum = 0;
    int64_t left_min = 0;
    int64_t right_sum = 0;
    int64_t right_min = 0;

    if (!validate_rec(
            tree,node*2,left,mid,&left_sum,&left_min
        ) ||
        !validate_rec(
            tree,node*2+1,mid,right,&right_sum,&right_min
        )) {
        return false;
    }

    const int64_t expected_sum = left_sum + right_sum;
    const int64_t expected_min = min_i64(left_min, right_min);

    if (tree->sum[node] != expected_sum ||
        tree->min[node] != expected_min) {
        return false;
    }

    *out_sum = expected_sum;
    *out_min = expected_min;
    return true;
}

bool int_segment_tree_validate(const IntSegmentTree *tree) {
    if (tree == NULL) return false;

    if (tree->count == 0) {
        return tree->sum == NULL &&
               tree->min == NULL &&
               tree->capacity == 0;
    }

    if (tree->sum == NULL || tree->min == NULL ||
        tree->capacity == 0) {
        return false;
    }

    int64_t sum = 0;
    int64_t min = 0;

    return validate_rec(
        tree,1,0,tree->count,&sum,&min
    );
}
