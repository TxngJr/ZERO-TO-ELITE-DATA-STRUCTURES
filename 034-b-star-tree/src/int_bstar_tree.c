#include "int_bstar_tree.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct BStarNode {
    bool leaf;
    size_t count;
    int *keys;
    struct BStarNode **children;
} BStarNode;

struct IntBStarTree {
    size_t order;
    size_t size;
    BStarNode *root;
};

typedef struct {
    BStarNode **spares;
    size_t spare_count;
    size_t spare_used;
    int *temp_keys;
    BStarNode **temp_children;
} InsertContext;

static size_t max_keys(const IntBStarTree *tree) {
    return tree->order - 1U;
}

static size_t min_keys(const IntBStarTree *tree) {
    return (2U * tree->order) / 3U - 1U;
}

static BStarNode *node_create(size_t order, bool leaf) {
    if (order < 3 || order > (SIZE_MAX - 1U)) return NULL;

    BStarNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    if (order > SIZE_MAX / sizeof *node->keys ||
        order + 1U > SIZE_MAX / sizeof *node->children) {
        free(node);
        return NULL;
    }

    node->keys = malloc(order * sizeof *node->keys);
    node->children = calloc(order + 1U, sizeof *node->children);

    if (node->keys == NULL || node->children == NULL) {
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }

    node->leaf = leaf;
    return node;
}

static void node_free_shallow(BStarNode *node) {
    if (node == NULL) return;
    free(node->keys);
    free(node->children);
    free(node);
}

static void node_free_deep(BStarNode *node) {
    if (node == NULL) return;

    if (!node->leaf) {
        for (size_t i = 0; i <= node->count; ++i) {
            node_free_deep(node->children[i]);
        }
    }

    node_free_shallow(node);
}

IntBStarTree *int_bstar_tree_create(size_t order) {
    if (order < 3 || order % 3U != 0U) return NULL;

    IntBStarTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->order = order;
    tree->root = node_create(order, true);

    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }

    return tree;
}

void int_bstar_tree_free(IntBStarTree *tree) {
    if (tree == NULL) return;
    node_free_deep(tree->root);
    free(tree);
}

size_t int_bstar_tree_size(const IntBStarTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

size_t int_bstar_tree_order(const IntBStarTree *tree) {
    return tree == NULL ? 0 : tree->order;
}

static size_t node_count_rec(const BStarNode *node) {
    if (node == NULL) return 0;

    size_t count = 1;

    if (!node->leaf) {
        for (size_t i = 0; i <= node->count; ++i) {
            count += node_count_rec(node->children[i]);
        }
    }

    return count;
}

size_t int_bstar_tree_node_count(const IntBStarTree *tree) {
    return tree == NULL ? 0 : node_count_rec(tree->root);
}

double int_bstar_tree_utilization(const IntBStarTree *tree) {
    if (tree == NULL || tree->size == 0) return 0.0;

    const size_t nodes = int_bstar_tree_node_count(tree);
    const size_t capacity_per_node = max_keys(tree);

    if (nodes == 0 || capacity_per_node == 0) return 0.0;

    return (double)tree->size /
           ((double)nodes * (double)capacity_per_node);
}

static size_t find_index(const BStarNode *node, int key) {
    size_t index = 0;
    while (index < node->count && node->keys[index] < key) ++index;
    return index;
}

static bool node_contains(const BStarNode *node, int key) {
    const size_t index = find_index(node, key);

    if (index < node->count && node->keys[index] == key) return true;
    if (node->leaf) return false;

    return node_contains(node->children[index], key);
}

bool int_bstar_tree_contains(const IntBStarTree *tree, int key) {
    return tree != NULL &&
           tree->root != NULL &&
           node_contains(tree->root, key);
}

static size_t tree_node_height(const BStarNode *root) {
    size_t nodes = 1;
    const BStarNode *node = root;

    while (node != NULL && !node->leaf) {
        ++nodes;
        node = node->children[0];
    }

    return nodes;
}

static void reset_spare(
    const IntBStarTree *tree,
    BStarNode *node,
    bool leaf
) {
    node->leaf = leaf;
    node->count = 0;
    memset(
        node->children,
        0,
        (tree->order + 1U) * sizeof *node->children
    );
}

static BStarNode *take_spare(
    const IntBStarTree *tree,
    InsertContext *context,
    bool leaf
) {
    if (context->spare_used >= context->spare_count) return NULL;

    BStarNode *node = context->spares[context->spare_used++];
    reset_spare(tree, node, leaf);
    return node;
}

static bool prepare_insert_context(
    const IntBStarTree *tree,
    InsertContext *context
) {
    memset(context, 0, sizeof *context);

    const size_t height_nodes = tree_node_height(tree->root);
    const size_t spare_count = height_nodes + 1U;

    if (spare_count > SIZE_MAX / sizeof *context->spares) return false;

    context->spares = calloc(
        spare_count,
        sizeof *context->spares
    );
    if (context->spares == NULL) return false;

    context->spare_count = spare_count;

    for (size_t i = 0; i < spare_count; ++i) {
        context->spares[i] = node_create(tree->order, true);

        if (context->spares[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                node_free_shallow(context->spares[j]);
            }
            free(context->spares);
            memset(context, 0, sizeof *context);
            return false;
        }
    }

    const size_t temp_key_capacity = 2U * tree->order;
    const size_t temp_child_capacity = temp_key_capacity + 1U;

    if (temp_key_capacity > SIZE_MAX / sizeof *context->temp_keys ||
        temp_child_capacity > SIZE_MAX / sizeof *context->temp_children) {
        return false;
    }

    context->temp_keys = malloc(
        temp_key_capacity * sizeof *context->temp_keys
    );
    context->temp_children = malloc(
        temp_child_capacity * sizeof *context->temp_children
    );

    return context->temp_keys != NULL &&
           context->temp_children != NULL;
}

static void destroy_insert_context(
    InsertContext *context
) {
    if (context == NULL) return;

    for (size_t i = context->spare_used;
         i < context->spare_count;
         ++i) {
        node_free_shallow(context->spares[i]);
    }

    free(context->spares);
    free(context->temp_keys);
    free(context->temp_children);
    memset(context, 0, sizeof *context);
}

static size_t combine_pair(
    BStarNode *left,
    int separator,
    BStarNode *right,
    InsertContext *context
) {
    size_t key_index = 0;

    for (size_t i = 0; i < left->count; ++i) {
        context->temp_keys[key_index++] = left->keys[i];
    }

    context->temp_keys[key_index++] = separator;

    for (size_t i = 0; i < right->count; ++i) {
        context->temp_keys[key_index++] = right->keys[i];
    }

    if (!left->leaf) {
        size_t child_index = 0;

        for (size_t i = 0; i <= left->count; ++i) {
            context->temp_children[child_index++] = left->children[i];
        }

        for (size_t i = 0; i <= right->count; ++i) {
            context->temp_children[child_index++] = right->children[i];
        }
    }

    return key_index;
}

static void clear_children(
    const IntBStarTree *tree,
    BStarNode *node
) {
    memset(
        node->children,
        0,
        (tree->order + 1U) * sizeof *node->children
    );
}

static void load_segment(
    const IntBStarTree *tree,
    BStarNode *node,
    size_t key_start,
    size_t key_count,
    size_t child_start,
    InsertContext *context
) {
    for (size_t i = 0; i < key_count; ++i) {
        node->keys[i] = context->temp_keys[key_start + i];
    }

    if (!node->leaf) {
        clear_children(tree, node);

        for (size_t i = 0; i <= key_count; ++i) {
            node->children[i] =
                context->temp_children[child_start + i];
        }
    }

    node->count = key_count;
}

static void redistribute_pair(
    IntBStarTree *tree,
    BStarNode *parent,
    size_t separator_index,
    BStarNode *left,
    BStarNode *right,
    InsertContext *context
) {
    const size_t total = combine_pair(
        left,
        parent->keys[separator_index],
        right,
        context
    );

    const size_t left_count = total / 2U;
    const size_t right_count = total - left_count - 1U;

    const int new_separator = context->temp_keys[left_count];

    load_segment(
        tree,
        left,
        0,
        left_count,
        0,
        context
    );

    load_segment(
        tree,
        right,
        left_count + 1U,
        right_count,
        left_count + 1U,
        context
    );

    parent->keys[separator_index] = new_separator;
}

static bool split_pair_three(
    IntBStarTree *tree,
    BStarNode *parent,
    size_t separator_index,
    BStarNode *left,
    BStarNode *right,
    InsertContext *context
) {
    BStarNode *third = take_spare(
        tree,
        context,
        left->leaf
    );
    if (third == NULL) return false;

    const size_t total = combine_pair(
        left,
        parent->keys[separator_index],
        right,
        context
    );

    const size_t minimum = min_keys(tree);
    const size_t available = total - 2U;

    const size_t first_count = minimum;
    const size_t second_count = minimum;
    const size_t third_count =
        available - first_count - second_count;

    if (third_count < minimum ||
        third_count > max_keys(tree)) {
        return false;
    }

    const size_t first_separator_index = first_count;
    const size_t second_separator_index =
        first_count + 1U + second_count;

    const int first_separator =
        context->temp_keys[first_separator_index];
    const int second_separator =
        context->temp_keys[second_separator_index];

    load_segment(
        tree,
        left,
        0,
        first_count,
        0,
        context
    );

    load_segment(
        tree,
        right,
        first_separator_index + 1U,
        second_count,
        first_separator_index + 1U,
        context
    );

    load_segment(
        tree,
        third,
        second_separator_index + 1U,
        third_count,
        second_separator_index + 1U,
        context
    );

    for (size_t i = parent->count + 1U;
         i > separator_index + 2U;
         --i) {
        parent->children[i] = parent->children[i - 1U];
    }

    parent->children[separator_index] = left;
    parent->children[separator_index + 1U] = right;
    parent->children[separator_index + 2U] = third;

    for (size_t i = parent->count;
         i > separator_index + 1U;
         --i) {
        parent->keys[i] = parent->keys[i - 1U];
    }

    parent->keys[separator_index] = first_separator;
    parent->keys[separator_index + 1U] = second_separator;
    ++parent->count;

    return true;
}

static bool fix_child_overflow(
    IntBStarTree *tree,
    BStarNode *parent,
    size_t child_index,
    InsertContext *context
) {
    BStarNode *child = parent->children[child_index];
    const size_t maximum = max_keys(tree);

    if (child->count <= maximum) return true;

    if (child_index > 0U) {
        BStarNode *left = parent->children[child_index - 1U];

        if (left->count < maximum) {
            redistribute_pair(
                tree,
                parent,
                child_index - 1U,
                left,
                child,
                context
            );
            return true;
        }
    }

    if (child_index < parent->count) {
        BStarNode *right = parent->children[child_index + 1U];

        if (right->count < maximum) {
            redistribute_pair(
                tree,
                parent,
                child_index,
                child,
                right,
                context
            );
            return true;
        }
    }

    if (child_index > 0U) {
        return split_pair_three(
            tree,
            parent,
            child_index - 1U,
            parent->children[child_index - 1U],
            child,
            context
        );
    }

    return split_pair_three(
        tree,
        parent,
        child_index,
        child,
        parent->children[child_index + 1U],
        context
    );
}

static bool insert_rec(
    IntBStarTree *tree,
    BStarNode *node,
    int key,
    InsertContext *context
) {
    size_t index = find_index(node, key);

    if (node->leaf) {
        for (size_t i = node->count; i > index; --i) {
            node->keys[i] = node->keys[i - 1U];
        }

        node->keys[index] = key;
        ++node->count;
        return true;
    }

    if (!insert_rec(
            tree,
            node->children[index],
            key,
            context
        )) {
        return false;
    }

    return fix_child_overflow(
        tree,
        node,
        index,
        context
    );
}

static bool split_root(
    IntBStarTree *tree,
    InsertContext *context
) {
    BStarNode *root = tree->root;

    if (root->count <= max_keys(tree)) return true;

    BStarNode *left = take_spare(
        tree,
        context,
        root->leaf
    );
    BStarNode *right = take_spare(
        tree,
        context,
        root->leaf
    );

    if (left == NULL || right == NULL) return false;

    const size_t median = root->count / 2U;
    const int promoted = root->keys[median];

    left->count = median;
    right->count = root->count - median - 1U;

    for (size_t i = 0; i < left->count; ++i) {
        left->keys[i] = root->keys[i];
    }

    for (size_t i = 0; i < right->count; ++i) {
        right->keys[i] = root->keys[median + 1U + i];
    }

    if (!root->leaf) {
        for (size_t i = 0; i <= left->count; ++i) {
            left->children[i] = root->children[i];
        }

        for (size_t i = 0; i <= right->count; ++i) {
            right->children[i] =
                root->children[median + 1U + i];
        }
    }

    clear_children(tree, root);

    root->leaf = false;
    root->count = 1;
    root->keys[0] = promoted;
    root->children[0] = left;
    root->children[1] = right;

    return true;
}

bool int_bstar_tree_insert(IntBStarTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (tree->size == SIZE_MAX) return false;
    if (int_bstar_tree_contains(tree, key)) return false;

    InsertContext context;

    if (!prepare_insert_context(tree, &context)) {
        destroy_insert_context(&context);
        return false;
    }

    const bool inserted = insert_rec(
        tree,
        tree->root,
        key,
        &context
    );

    const bool root_ok =
        inserted && split_root(tree, &context);

    if (root_ok) {
        ++tree->size;
    }

    destroy_insert_context(&context);
    return root_ok;
}

static void inorder_rec(
    const BStarNode *node,
    int *output,
    size_t *index
) {
    for (size_t i = 0; i < node->count; ++i) {
        if (!node->leaf) {
            inorder_rec(node->children[i], output, index);
        }

        output[(*index)++] = node->keys[i];
    }

    if (!node->leaf) {
        inorder_rec(node->children[node->count], output, index);
    }
}

bool int_bstar_tree_inorder(
    const IntBStarTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) return false;

    if (tree->size > 0 &&
        (output == NULL || capacity < tree->size)) {
        return false;
    }

    *out_written = 0;
    inorder_rec(tree->root, output, out_written);
    return true;
}

bool int_bstar_tree_remove(IntBStarTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (!int_bstar_tree_contains(tree, key)) return false;

    if (tree->size == 1U) {
        tree->root->count = 0;
        tree->root->leaf = true;
        clear_children(tree, tree->root);
        tree->size = 0;
        return true;
    }

    if (tree->size > SIZE_MAX / sizeof(int)) return false;

    int *keys = malloc(tree->size * sizeof *keys);
    if (keys == NULL) return false;

    size_t written = 0;

    if (!int_bstar_tree_inorder(
            tree,
            keys,
            tree->size,
            &written
        ) ||
        written != tree->size) {
        free(keys);
        return false;
    }

    IntBStarTree *replacement =
        int_bstar_tree_create(tree->order);

    if (replacement == NULL) {
        free(keys);
        return false;
    }

    bool ok = true;

    for (size_t i = 0; i < written; ++i) {
        if (keys[i] == key) continue;

        if (!int_bstar_tree_insert(replacement, keys[i])) {
            ok = false;
            break;
        }
    }

    free(keys);

    if (!ok) {
        int_bstar_tree_free(replacement);
        return false;
    }

    BStarNode *old_root = tree->root;
    tree->root = replacement->root;
    tree->size = replacement->size;

    replacement->root = NULL;
    int_bstar_tree_free(replacement);
    node_free_deep(old_root);

    return true;
}

bool int_bstar_tree_height(
    const IntBStarTree *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL ||
        out_edge_height == NULL || tree->size == 0) {
        return false;
    }

    size_t height = 0;
    const BStarNode *node = tree->root;

    while (!node->leaf) {
        ++height;
        node = node->children[0];
    }

    *out_edge_height = height;
    return true;
}

static bool validate_node(
    const IntBStarTree *tree,
    const BStarNode *node,
    bool is_root,
    bool parent_is_one_key_root,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t depth,
    bool *leaf_depth_set,
    size_t *leaf_depth,
    size_t *key_count
) {
    if (node == NULL) return false;
    if (node->count > max_keys(tree)) return false;

    if (!is_root) {
        size_t minimum = min_keys(tree);

        if (parent_is_one_key_root) {
            minimum = (tree->order - 1U) / 2U;
        }

        if (node->count < minimum) return false;
    }

    if (is_root && tree->size > 0 && node->count == 0) {
        return false;
    }

    for (size_t i = 1; i < node->count; ++i) {
        if (node->keys[i - 1U] >= node->keys[i]) {
            return false;
        }
    }

    if (node->count > 0) {
        if (has_low && node->keys[0] <= low) return false;
        if (has_high &&
            node->keys[node->count - 1U] >= high) {
            return false;
        }
    }

    *key_count += node->count;

    if (*key_count > tree->size) return false;

    if (node->leaf) {
        if (!*leaf_depth_set) {
            *leaf_depth_set = true;
            *leaf_depth = depth;
        } else if (*leaf_depth != depth) {
            return false;
        }

        return true;
    }

    for (size_t i = 0; i <= node->count; ++i) {
        if (node->children[i] == NULL) return false;

        bool child_has_low = has_low;
        int child_low = low;
        bool child_has_high = has_high;
        int child_high = high;

        if (i > 0) {
            child_has_low = true;
            child_low = node->keys[i - 1U];
        }

        if (i < node->count) {
            child_has_high = true;
            child_high = node->keys[i];
        }

        if (!validate_node(
                tree,
                node->children[i],
                false,
                is_root && node->count == 1U,
                child_has_low,
                child_low,
                child_has_high,
                child_high,
                depth + 1U,
                leaf_depth_set,
                leaf_depth,
                key_count
            )) {
            return false;
        }
    }

    return true;
}

bool int_bstar_tree_validate(const IntBStarTree *tree) {
    if (tree == NULL || tree->root == NULL) return false;
    if (tree->order < 3 || tree->order % 3U != 0U) return false;

    if (tree->size == 0) {
        return tree->root->leaf &&
               tree->root->count == 0;
    }

    bool leaf_depth_set = false;
    size_t leaf_depth = 0;
    size_t key_count = 0;

    return validate_node(
        tree,
        tree->root,
        true,
        false,
        false,
        INT_MIN,
        false,
        INT_MAX,
        0,
        &leaf_depth_set,
        &leaf_depth,
        &key_count
    ) && key_count == tree->size;
}
