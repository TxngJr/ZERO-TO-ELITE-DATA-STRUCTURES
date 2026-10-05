#include "int_bplus_tree.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct BPlusNode {
    bool leaf;
    size_t count;
    int *keys;
    struct BPlusNode **children;
    struct BPlusNode *next;
} BPlusNode;

struct IntBPlusTree {
    size_t t;
    size_t size;
    BPlusNode *root;
};

static size_t max_keys(const IntBPlusTree *tree) {
    return 2U * tree->t - 1U;
}

static BPlusNode *node_create(size_t t, bool leaf) {
    if (t < 2 || t > (SIZE_MAX - 1U) / 2U) return NULL;

    BPlusNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    const size_t key_capacity = 2U * t;
    const size_t child_capacity = 2U * t + 1U;

    if (key_capacity > SIZE_MAX / sizeof *node->keys ||
        child_capacity > SIZE_MAX / sizeof *node->children) {
        free(node);
        return NULL;
    }

    node->keys = malloc(key_capacity * sizeof *node->keys);
    node->children = calloc(child_capacity, sizeof *node->children);

    if (node->keys == NULL || node->children == NULL) {
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }

    node->leaf = leaf;
    return node;
}

static void node_free(BPlusNode *node) {
    if (node == NULL) return;

    if (!node->leaf) {
        for (size_t i = 0; i <= node->count; ++i) {
            node_free(node->children[i]);
        }
    }

    free(node->keys);
    free(node->children);
    free(node);
}

IntBPlusTree *int_bplus_tree_create(size_t minimum_degree) {
    if (minimum_degree < 2) return NULL;

    IntBPlusTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->t = minimum_degree;
    tree->root = node_create(minimum_degree, true);

    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }

    return tree;
}

void int_bplus_tree_free(IntBPlusTree *tree) {
    if (tree == NULL) return;
    node_free(tree->root);
    free(tree);
}

size_t int_bplus_tree_size(const IntBPlusTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

static int subtree_min(const BPlusNode *node) {
    while (!node->leaf) node = node->children[0];
    return node->keys[0];
}

static void recompute_separators(BPlusNode *node) {
    if (node == NULL || node->leaf) return;

    for (size_t i = 0; i < node->count; ++i) {
        node->keys[i] = subtree_min(node->children[i + 1]);
    }
}

static size_t choose_child(const BPlusNode *node, int key) {
    size_t i = 0;
    while (i < node->count && key >= node->keys[i]) ++i;
    return i;
}

static BPlusNode *find_leaf(const IntBPlusTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return NULL;

    BPlusNode *node = tree->root;

    while (!node->leaf) {
        node = node->children[choose_child(node, key)];
    }

    return node;
}

bool int_bplus_tree_contains(const IntBPlusTree *tree, int key) {
    BPlusNode *leaf = find_leaf(tree, key);
    if (leaf == NULL) return false;

    size_t i = 0;
    while (i < leaf->count && leaf->keys[i] < key) ++i;

    return i < leaf->count && leaf->keys[i] == key;
}

typedef struct {
    bool split;
    BPlusNode *right;
} InsertResult;

static InsertResult insert_rec(
    IntBPlusTree *tree,
    BPlusNode *node,
    int key
) {
    InsertResult result = {0};

    if (node->leaf) {
        size_t pos = 0;
        while (pos < node->count && node->keys[pos] < key) ++pos;

        for (size_t i = node->count; i > pos; --i) {
            node->keys[i] = node->keys[i - 1];
        }

        node->keys[pos] = key;
        ++node->count;

        if (node->count <= max_keys(tree)) return result;

        BPlusNode *right = node_create(tree->t, true);
        if (right == NULL) {
            for (size_t i = pos + 1; i < node->count; ++i) {
                node->keys[i - 1] = node->keys[i];
            }
            --node->count;
            return result;
        }

        const size_t left_count = tree->t;
        const size_t right_count = node->count - left_count;

        for (size_t i = 0; i < right_count; ++i) {
            right->keys[i] = node->keys[left_count + i];
        }

        right->count = right_count;
        node->count = left_count;

        right->next = node->next;
        node->next = right;

        result.split = true;
        result.right = right;
        return result;
    }

    const size_t child_index = choose_child(node, key);
    InsertResult child_result = insert_rec(
        tree,
        node->children[child_index],
        key
    );

    if (!child_result.split) {
        recompute_separators(node);
        return result;
    }

    for (size_t i = node->count + 1; i > child_index + 1; --i) {
        node->children[i] = node->children[i - 1];
    }

    node->children[child_index + 1] = child_result.right;
    ++node->count;
    recompute_separators(node);

    if (node->count <= max_keys(tree)) return result;

    BPlusNode *right = node_create(tree->t, false);
    if (right == NULL) {
        return result;
    }

    const size_t total_children = node->count + 1;
    const size_t left_children = tree->t;
    const size_t right_children = total_children - left_children;

    for (size_t i = 0; i < right_children; ++i) {
        right->children[i] = node->children[left_children + i];
        node->children[left_children + i] = NULL;
    }

    node->count = left_children - 1;
    right->count = right_children - 1;

    recompute_separators(node);
    recompute_separators(right);

    result.split = true;
    result.right = right;
    return result;
}

bool int_bplus_tree_insert(IntBPlusTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (int_bplus_tree_contains(tree, key)) return false;
    if (tree->size == SIZE_MAX) return false;

    InsertResult result = insert_rec(tree, tree->root, key);

    if (result.split) {
        BPlusNode *new_root = node_create(tree->t, false);
        if (new_root == NULL) {
            return false;
        }

        new_root->children[0] = tree->root;
        new_root->children[1] = result.right;
        new_root->count = 1;
        recompute_separators(new_root);
        tree->root = new_root;
    }

    ++tree->size;
    return true;
}

static void remove_leaf_key(BPlusNode *leaf, size_t pos) {
    for (size_t i = pos + 1; i < leaf->count; ++i) {
        leaf->keys[i - 1] = leaf->keys[i];
    }
    --leaf->count;
}

static void borrow_leaf_from_left(
    BPlusNode *left,
    BPlusNode *child
) {
    for (size_t i = child->count; i > 0; --i) {
        child->keys[i] = child->keys[i - 1];
    }

    child->keys[0] = left->keys[left->count - 1];
    --left->count;
    ++child->count;
}

static void borrow_leaf_from_right(
    BPlusNode *child,
    BPlusNode *right
) {
    child->keys[child->count++] = right->keys[0];

    for (size_t i = 1; i < right->count; ++i) {
        right->keys[i - 1] = right->keys[i];
    }

    --right->count;
}

static void borrow_internal_from_left(
    BPlusNode *left,
    BPlusNode *child
) {
    const size_t child_children = child->count + 1;

    for (size_t i = child_children; i > 0; --i) {
        child->children[i] = child->children[i - 1];
    }

    child->children[0] = left->children[left->count];
    left->children[left->count] = NULL;

    --left->count;
    ++child->count;

    recompute_separators(left);
    recompute_separators(child);
}

static void borrow_internal_from_right(
    BPlusNode *child,
    BPlusNode *right
) {
    child->children[child->count + 1] = right->children[0];
    ++child->count;

    for (size_t i = 1; i <= right->count; ++i) {
        right->children[i - 1] = right->children[i];
    }

    right->children[right->count] = NULL;
    --right->count;

    recompute_separators(child);
    recompute_separators(right);
}

static void free_single_node(BPlusNode *node) {
    free(node->keys);
    free(node->children);
    free(node);
}

static void merge_nodes(
    BPlusNode *left,
    BPlusNode *right
) {
    if (left->leaf) {
        for (size_t i = 0; i < right->count; ++i) {
            left->keys[left->count + i] = right->keys[i];
        }

        left->count += right->count;
        left->next = right->next;
        free_single_node(right);
        return;
    }

    const size_t left_children = left->count + 1;
    const size_t right_children = right->count + 1;

    for (size_t i = 0; i < right_children; ++i) {
        left->children[left_children + i] = right->children[i];
        right->children[i] = NULL;
    }

    left->count = left_children + right_children - 1;
    recompute_separators(left);
    free_single_node(right);
}

static void remove_child_pointer(
    BPlusNode *parent,
    size_t remove_index
) {
    const size_t child_count = parent->count + 1;

    for (size_t i = remove_index + 1; i < child_count; ++i) {
        parent->children[i - 1] = parent->children[i];
    }

    parent->children[child_count - 1] = NULL;
    --parent->count;
    recompute_separators(parent);
}

static bool delete_rec(
    IntBPlusTree *tree,
    BPlusNode *node,
    int key,
    bool is_root
) {
    if (node->leaf) {
        size_t pos = 0;
        while (pos < node->count && node->keys[pos] < key) ++pos;

        if (pos >= node->count || node->keys[pos] != key) return false;

        remove_leaf_key(node, pos);
        return true;
    }

    size_t index = choose_child(node, key);

    if (!delete_rec(tree, node->children[index], key, false)) {
        return false;
    }

    const size_t min_keys = tree->t - 1;
    BPlusNode *child = node->children[index];

    if (child->count < min_keys) {
        BPlusNode *left = index > 0 ? node->children[index - 1] : NULL;
        BPlusNode *right =
            index < node->count ? node->children[index + 1] : NULL;

        if (left != NULL && left->count > min_keys) {
            if (child->leaf) {
                borrow_leaf_from_left(left, child);
            } else {
                borrow_internal_from_left(left, child);
            }
        } else if (right != NULL && right->count > min_keys) {
            if (child->leaf) {
                borrow_leaf_from_right(child, right);
            } else {
                borrow_internal_from_right(child, right);
            }
        } else if (left != NULL) {
            merge_nodes(left, child);
            remove_child_pointer(node, index);
            child = left;
        } else if (right != NULL) {
            merge_nodes(child, right);
            remove_child_pointer(node, index + 1);
        }
    }

    recompute_separators(node);
    (void)is_root;
    return true;
}

bool int_bplus_tree_remove(IntBPlusTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (!int_bplus_tree_contains(tree, key)) return false;

    if (!delete_rec(tree, tree->root, key, true)) return false;

    if (!tree->root->leaf && tree->root->count == 0) {
        BPlusNode *old_root = tree->root;
        tree->root = old_root->children[0];
        old_root->children[0] = NULL;
        free_single_node(old_root);
    }

    --tree->size;
    return true;
}

bool int_bplus_tree_range(
    const IntBPlusTree *tree,
    int low,
    int high,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) return false;

    *out_written = 0;
    if (low > high || tree->size == 0) return true;

    BPlusNode *leaf = find_leaf(tree, low);

    while (leaf != NULL) {
        for (size_t i = 0; i < leaf->count; ++i) {
            const int key = leaf->keys[i];

            if (key < low) continue;
            if (key > high) return true;

            if (*out_written >= capacity || output == NULL) {
                return false;
            }

            output[(*out_written)++] = key;
        }

        leaf = leaf->next;
    }

    return true;
}

bool int_bplus_tree_height(
    const IntBPlusTree *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL ||
        out_edge_height == NULL || tree->size == 0) {
        return false;
    }

    size_t height = 0;
    BPlusNode *node = tree->root;

    while (!node->leaf) {
        ++height;
        node = node->children[0];
    }

    *out_edge_height = height;
    return true;
}

typedef struct {
    const BPlusNode **leaves;
    size_t count;
    size_t capacity;
    size_t key_count;
    bool leaf_depth_set;
    size_t leaf_depth;
} ValidateState;

static bool push_leaf(ValidateState *state, const BPlusNode *leaf) {
    if (state->count == state->capacity) {
        size_t next_capacity = state->capacity == 0 ? 16 : state->capacity * 2;
        const BPlusNode **next = realloc(
            state->leaves,
            next_capacity * sizeof *next
        );
        if (next == NULL) return false;

        state->leaves = next;
        state->capacity = next_capacity;
    }

    state->leaves[state->count++] = leaf;
    return true;
}

static bool validate_node(
    const IntBPlusTree *tree,
    const BPlusNode *node,
    bool is_root,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t depth,
    ValidateState *state
) {
    if (node == NULL) return false;
    if (node->count > max_keys(tree)) return false;

    if (!is_root && node->count < tree->t - 1) return false;

    for (size_t i = 1; i < node->count; ++i) {
        if (node->keys[i - 1] >= node->keys[i]) return false;
    }

    if (node->leaf) {
        if (!state->leaf_depth_set) {
            state->leaf_depth_set = true;
            state->leaf_depth = depth;
        } else if (state->leaf_depth != depth) {
            return false;
        }

        if (node->count > 0) {
            if (has_low && node->keys[0] < low) return false;
            if (has_high && node->keys[node->count - 1] >= high) return false;
        }

        state->key_count += node->count;
        if (state->key_count > tree->size) return false;

        return push_leaf(state, node);
    }

    if (is_root && tree->size > 0 && node->count == 0) return false;

    for (size_t i = 0; i <= node->count; ++i) {
        if (node->children[i] == NULL) return false;
    }

    for (size_t i = 0; i < node->count; ++i) {
        if (node->keys[i] != subtree_min(node->children[i + 1])) {
            return false;
        }
    }

    for (size_t i = 0; i <= node->count; ++i) {
        bool child_has_low = has_low;
        int child_low = low;
        bool child_has_high = has_high;
        int child_high = high;

        if (i > 0) {
            child_has_low = true;
            child_low = node->keys[i - 1];
        }

        if (i < node->count) {
            child_has_high = true;
            child_high = node->keys[i];
        }

        if (!validate_node(
                tree,
                node->children[i],
                false,
                child_has_low,
                child_low,
                child_has_high,
                child_high,
                depth + 1,
                state
            )) {
            return false;
        }
    }

    return true;
}

bool int_bplus_tree_validate(const IntBPlusTree *tree) {
    if (tree == NULL || tree->root == NULL || tree->t < 2) return false;

    if (tree->size == 0) {
        return tree->root->leaf && tree->root->count == 0;
    }

    ValidateState state = {0};

    const bool structure_ok = validate_node(
        tree,
        tree->root,
        true,
        false,
        INT_MIN,
        false,
        INT_MAX,
        0,
        &state
    );

    if (!structure_ok || state.key_count != tree->size || state.count == 0) {
        free(state.leaves);
        return false;
    }

    for (size_t i = 0; i < state.count; ++i) {
        const BPlusNode *expected_next =
            i + 1 < state.count ? state.leaves[i + 1] : NULL;

        if (state.leaves[i]->next != expected_next) {
            free(state.leaves);
            return false;
        }

        if (i + 1 < state.count &&
            state.leaves[i]->count > 0 &&
            state.leaves[i + 1]->count > 0 &&
            state.leaves[i]->keys[state.leaves[i]->count - 1] >=
                state.leaves[i + 1]->keys[0]) {
            free(state.leaves);
            return false;
        }
    }

    free(state.leaves);
    return true;
}
