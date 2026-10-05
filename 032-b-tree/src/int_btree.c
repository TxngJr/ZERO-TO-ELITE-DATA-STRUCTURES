#include "int_btree.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct BTreeNode {
    bool leaf;
    size_t count;
    int *keys;
    struct BTreeNode **children;
} BTreeNode;

struct IntBTree {
    size_t t;
    size_t size;
    BTreeNode *root;
};

static BTreeNode *node_create(size_t t, bool leaf) {
    BTreeNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    if (t > SIZE_MAX / 2U) {
        free(node);
        return NULL;
    }

    const size_t max_keys = 2U * t - 1U;
    const size_t max_children = 2U * t;

    if (max_keys > SIZE_MAX / sizeof *node->keys ||
        max_children > SIZE_MAX / sizeof *node->children) {
        free(node);
        return NULL;
    }

    node->keys = malloc(max_keys * sizeof *node->keys);
    node->children = calloc(max_children, sizeof *node->children);

    if (node->keys == NULL || node->children == NULL) {
        free(node->keys);
        free(node->children);
        free(node);
        return NULL;
    }

    node->leaf = leaf;
    return node;
}

static void node_free(BTreeNode *node, size_t t) {
    if (node == NULL) return;

    if (!node->leaf) {
        for (size_t i = 0; i <= node->count; ++i) {
            node_free(node->children[i], t);
        }
    }

    free(node->keys);
    free(node->children);
    free(node);
    (void)t;
}

IntBTree *int_btree_create(size_t minimum_degree) {
    if (minimum_degree < 2) return NULL;

    IntBTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->t = minimum_degree;
    tree->root = node_create(minimum_degree, true);

    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }

    return tree;
}

void int_btree_free(IntBTree *tree) {
    if (tree == NULL) return;
    node_free(tree->root, tree->t);
    free(tree);
}

size_t int_btree_size(const IntBTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

size_t int_btree_minimum_degree(const IntBTree *tree) {
    return tree == NULL ? 0 : tree->t;
}

static bool node_contains(const BTreeNode *node, int key) {
    size_t i = 0;

    while (i < node->count && key > node->keys[i]) ++i;

    if (i < node->count && key == node->keys[i]) return true;
    if (node->leaf) return false;

    return node_contains(node->children[i], key);
}

bool int_btree_contains(const IntBTree *tree, int key) {
    return tree != NULL &&
           tree->root != NULL &&
           node_contains(tree->root, key);
}

static bool split_child(
    IntBTree *tree,
    BTreeNode *parent,
    size_t index
) {
    const size_t t = tree->t;
    BTreeNode *left = parent->children[index];
    BTreeNode *right = node_create(t, left->leaf);

    if (right == NULL) return false;

    right->count = t - 1;

    for (size_t j = 0; j < t - 1; ++j) {
        right->keys[j] = left->keys[j + t];
    }

    if (!left->leaf) {
        for (size_t j = 0; j < t; ++j) {
            right->children[j] = left->children[j + t];
            left->children[j + t] = NULL;
        }
    }

    const int median = left->keys[t - 1];
    left->count = t - 1;

    for (size_t j = parent->count + 1; j > index + 1; --j) {
        parent->children[j] = parent->children[j - 1];
    }

    parent->children[index + 1] = right;

    for (size_t j = parent->count; j > index; --j) {
        parent->keys[j] = parent->keys[j - 1];
    }

    parent->keys[index] = median;
    ++parent->count;
    return true;
}

static bool insert_nonfull(
    IntBTree *tree,
    BTreeNode *node,
    int key
) {
    size_t i = node->count;

    if (node->leaf) {
        while (i > 0 && key < node->keys[i - 1]) {
            node->keys[i] = node->keys[i - 1];
            --i;
        }

        node->keys[i] = key;
        ++node->count;
        return true;
    }

    while (i > 0 && key < node->keys[i - 1]) --i;

    if (node->children[i]->count == 2U * tree->t - 1U) {
        if (!split_child(tree, node, i)) return false;

        if (key > node->keys[i]) ++i;
    }

    return insert_nonfull(tree, node->children[i], key);
}

bool int_btree_insert(IntBTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (int_btree_contains(tree, key)) return false;
    if (tree->size == SIZE_MAX) return false;

    BTreeNode *root = tree->root;

    if (root->count == 2U * tree->t - 1U) {
        BTreeNode *new_root = node_create(tree->t, false);
        if (new_root == NULL) return false;

        new_root->children[0] = root;

        if (!split_child(tree, new_root, 0)) {
            new_root->children[0] = NULL;
            node_free(new_root, tree->t);
            return false;
        }

        tree->root = new_root;

        if (!insert_nonfull(tree, new_root, key)) {
            return false;
        }
    } else {
        if (!insert_nonfull(tree, root, key)) return false;
    }

    ++tree->size;
    return true;
}

static size_t find_key_index(const BTreeNode *node, int key) {
    size_t index = 0;
    while (index < node->count && node->keys[index] < key) ++index;
    return index;
}

static int predecessor(BTreeNode *node) {
    while (!node->leaf) node = node->children[node->count];
    return node->keys[node->count - 1];
}

static int successor(BTreeNode *node) {
    while (!node->leaf) node = node->children[0];
    return node->keys[0];
}

static void remove_from_leaf(BTreeNode *node, size_t index) {
    for (size_t i = index + 1; i < node->count; ++i) {
        node->keys[i - 1] = node->keys[i];
    }
    --node->count;
}

static void merge_children(
    IntBTree *tree,
    BTreeNode *parent,
    size_t index
) {
    const size_t t = tree->t;
    BTreeNode *left = parent->children[index];
    BTreeNode *right = parent->children[index + 1];

    left->keys[t - 1] = parent->keys[index];

    for (size_t i = 0; i < right->count; ++i) {
        left->keys[i + t] = right->keys[i];
    }

    if (!left->leaf) {
        for (size_t i = 0; i <= right->count; ++i) {
            left->children[i + t] = right->children[i];
            right->children[i] = NULL;
        }
    }

    left->count += right->count + 1;

    for (size_t i = index + 1; i < parent->count; ++i) {
        parent->keys[i - 1] = parent->keys[i];
    }

    for (size_t i = index + 2; i <= parent->count; ++i) {
        parent->children[i - 1] = parent->children[i];
    }

    parent->children[parent->count] = NULL;
    --parent->count;

    free(right->keys);
    free(right->children);
    free(right);
}

static void borrow_from_prev(
    BTreeNode *parent,
    size_t index
) {
    BTreeNode *child = parent->children[index];
    BTreeNode *sibling = parent->children[index - 1];

    for (size_t i = child->count; i > 0; --i) {
        child->keys[i] = child->keys[i - 1];
    }

    if (!child->leaf) {
        for (size_t i = child->count + 1; i > 0; --i) {
            child->children[i] = child->children[i - 1];
        }
    }

    child->keys[0] = parent->keys[index - 1];

    if (!child->leaf) {
        child->children[0] = sibling->children[sibling->count];
        sibling->children[sibling->count] = NULL;
    }

    parent->keys[index - 1] = sibling->keys[sibling->count - 1];

    ++child->count;
    --sibling->count;
}

static void borrow_from_next(
    BTreeNode *parent,
    size_t index
) {
    BTreeNode *child = parent->children[index];
    BTreeNode *sibling = parent->children[index + 1];

    child->keys[child->count] = parent->keys[index];

    if (!child->leaf) {
        child->children[child->count + 1] = sibling->children[0];
    }

    parent->keys[index] = sibling->keys[0];

    for (size_t i = 1; i < sibling->count; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->leaf) {
        for (size_t i = 1; i <= sibling->count; ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
        sibling->children[sibling->count] = NULL;
    }

    ++child->count;
    --sibling->count;
}

static void fill_child(
    IntBTree *tree,
    BTreeNode *parent,
    size_t index
) {
    const size_t t = tree->t;

    if (index > 0 &&
        parent->children[index - 1]->count >= t) {
        borrow_from_prev(parent, index);
    } else if (index < parent->count &&
               parent->children[index + 1]->count >= t) {
        borrow_from_next(parent, index);
    } else {
        if (index < parent->count) {
            merge_children(tree, parent, index);
        } else {
            merge_children(tree, parent, index - 1);
        }
    }
}

static void remove_from_node(
    IntBTree *tree,
    BTreeNode *node,
    int key
);

static void remove_from_internal(
    IntBTree *tree,
    BTreeNode *node,
    size_t index
) {
    const int key = node->keys[index];

    if (node->children[index]->count >= tree->t) {
        const int pred = predecessor(node->children[index]);
        node->keys[index] = pred;
        remove_from_node(tree, node->children[index], pred);
    } else if (node->children[index + 1]->count >= tree->t) {
        const int succ = successor(node->children[index + 1]);
        node->keys[index] = succ;
        remove_from_node(tree, node->children[index + 1], succ);
    } else {
        merge_children(tree, node, index);
        remove_from_node(tree, node->children[index], key);
    }
}

static void remove_from_node(
    IntBTree *tree,
    BTreeNode *node,
    int key
) {
    size_t index = find_key_index(node, key);

    if (index < node->count && node->keys[index] == key) {
        if (node->leaf) {
            remove_from_leaf(node, index);
        } else {
            remove_from_internal(tree, node, index);
        }
        return;
    }

    if (node->leaf) return;

    bool last_child = index == node->count;

    if (node->children[index]->count < tree->t) {
        fill_child(tree, node, index);
    }

    if (last_child && index > node->count) {
        remove_from_node(tree, node->children[index - 1], key);
    } else {
        remove_from_node(tree, node->children[index], key);
    }
}

bool int_btree_remove(IntBTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;
    if (!int_btree_contains(tree, key)) return false;

    remove_from_node(tree, tree->root, key);

    if (tree->root->count == 0 && !tree->root->leaf) {
        BTreeNode *old_root = tree->root;
        tree->root = old_root->children[0];
        old_root->children[0] = NULL;
        free(old_root->keys);
        free(old_root->children);
        free(old_root);
    }

    --tree->size;
    return true;
}

static void inorder_node(
    const BTreeNode *node,
    int *output,
    size_t *index
) {
    for (size_t i = 0; i < node->count; ++i) {
        if (!node->leaf) {
            inorder_node(node->children[i], output, index);
        }

        output[(*index)++] = node->keys[i];
    }

    if (!node->leaf) {
        inorder_node(node->children[node->count], output, index);
    }
}

bool int_btree_inorder(
    const IntBTree *tree,
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
    inorder_node(tree->root, output, out_written);
    return true;
}

bool int_btree_height(
    const IntBTree *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL ||
        out_edge_height == NULL || tree->size == 0) {
        return false;
    }

    size_t height = 0;
    const BTreeNode *node = tree->root;

    while (!node->leaf) {
        ++height;
        node = node->children[0];
    }

    *out_edge_height = height;
    return true;
}

static bool validate_node(
    const IntBTree *tree,
    const BTreeNode *node,
    bool is_root,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t depth,
    bool *leaf_depth_set,
    size_t *leaf_depth,
    size_t *key_count
) {
    const size_t max_keys = 2U * tree->t - 1U;

    if (node == NULL) return false;
    if (node->count > max_keys) return false;

    if (!is_root && node->count < tree->t - 1U) return false;

    if (is_root && tree->size > 0 && node->count == 0) return false;

    for (size_t i = 1; i < node->count; ++i) {
        if (node->keys[i - 1] >= node->keys[i]) return false;
    }

    if (node->count > 0) {
        if (has_low && node->keys[0] <= low) return false;
        if (has_high && node->keys[node->count - 1] >= high) return false;
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
                leaf_depth_set,
                leaf_depth,
                key_count
            )) {
            return false;
        }
    }

    return true;
}

bool int_btree_validate(const IntBTree *tree) {
    if (tree == NULL || tree->root == NULL || tree->t < 2) return false;

    if (tree->size == 0) {
        return tree->root->leaf && tree->root->count == 0;
    }

    bool leaf_depth_set = false;
    size_t leaf_depth = 0;
    size_t key_count = 0;

    return validate_node(
        tree,
        tree->root,
        true,
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
