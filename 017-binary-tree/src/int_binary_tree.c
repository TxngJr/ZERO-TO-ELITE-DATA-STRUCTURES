#include "int_binary_tree.h"

#include <stdint.h>
#include <stdlib.h>

struct IntBinaryNode {
    int value;
    struct IntBinaryNode *parent;
    struct IntBinaryNode *left;
    struct IntBinaryNode *right;
};

struct IntBinaryTree {
    IntBinaryNode *root;
    size_t size;
};

static IntBinaryNode *make_node(int value, IntBinaryNode *parent) {
    IntBinaryNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->value = value;
    node->parent = parent;
    return node;
}

static size_t free_subtree(IntBinaryNode *node) {
    if (node == NULL) return 0;

    const size_t left_count = free_subtree(node->left);
    const size_t right_count = free_subtree(node->right);

    free(node);
    return 1 + left_count + right_count;
}

IntBinaryTree *int_binary_tree_create(void) {
    return calloc(1, sizeof(IntBinaryTree));
}

void int_binary_tree_free(IntBinaryTree *tree) {
    if (tree == NULL) return;

    free_subtree(tree->root);
    free(tree);
}

size_t int_binary_tree_size(const IntBinaryTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

IntBinaryNode *int_binary_tree_root(const IntBinaryTree *tree) {
    return tree == NULL ? NULL : tree->root;
}

bool int_binary_tree_contains_node(
    const IntBinaryTree *tree,
    const IntBinaryNode *node
) {
    if (tree == NULL || node == NULL || tree->root == NULL) return false;

    const IntBinaryNode *cursor = node;

    for (size_t steps = 0; steps <= tree->size; ++steps) {
        if (cursor == tree->root) return true;
        if (cursor == NULL) return false;
        cursor = cursor->parent;
    }

    return false;
}

bool int_binary_tree_set_root(
    IntBinaryTree *tree,
    int value,
    IntBinaryNode **out_node
) {
    if (tree == NULL || tree->root != NULL || tree->size != 0) return false;

    IntBinaryNode *node = make_node(value, NULL);
    if (node == NULL) return false;

    tree->root = node;
    tree->size = 1;

    if (out_node != NULL) *out_node = node;
    return true;
}

static bool add_child(
    IntBinaryTree *tree,
    IntBinaryNode *parent,
    int value,
    bool left,
    IntBinaryNode **out_node
) {
    if (tree == NULL || parent == NULL || tree->size == SIZE_MAX) return false;
    if (!int_binary_tree_contains_node(tree, parent)) return false;

    IntBinaryNode **slot = left ? &parent->left : &parent->right;
    if (*slot != NULL) return false;

    IntBinaryNode *node = make_node(value, parent);
    if (node == NULL) return false;

    *slot = node;
    ++tree->size;

    if (out_node != NULL) *out_node = node;
    return true;
}

bool int_binary_tree_add_left(
    IntBinaryTree *tree,
    IntBinaryNode *parent,
    int value,
    IntBinaryNode **out_node
) {
    return add_child(tree, parent, value, true, out_node);
}

bool int_binary_tree_add_right(
    IntBinaryTree *tree,
    IntBinaryNode *parent,
    int value,
    IntBinaryNode **out_node
) {
    return add_child(tree, parent, value, false, out_node);
}

bool int_binary_tree_set_value(
    IntBinaryTree *tree,
    IntBinaryNode *node,
    int value
) {
    if (!int_binary_tree_contains_node(tree, node)) return false;
    node->value = value;
    return true;
}

bool int_binary_tree_remove_subtree(
    IntBinaryTree *tree,
    IntBinaryNode *node
) {
    if (tree == NULL || !int_binary_tree_contains_node(tree, node)) return false;

    if (node == tree->root) {
        tree->root = NULL;
    } else {
        IntBinaryNode *parent = node->parent;

        if (parent->left == node) parent->left = NULL;
        else if (parent->right == node) parent->right = NULL;
        else return false;
    }

    node->parent = NULL;
    const size_t removed = free_subtree(node);

    if (removed > tree->size) return false;
    tree->size -= removed;
    return true;
}

int int_binary_node_value(const IntBinaryNode *node) {
    return node == NULL ? 0 : node->value;
}

IntBinaryNode *int_binary_node_parent(const IntBinaryNode *node) {
    return node == NULL ? NULL : node->parent;
}

IntBinaryNode *int_binary_node_left(const IntBinaryNode *node) {
    return node == NULL ? NULL : node->left;
}

IntBinaryNode *int_binary_node_right(const IntBinaryNode *node) {
    return node == NULL ? NULL : node->right;
}

static size_t node_height(const IntBinaryNode *node) {
    if (node == NULL) return 0;

    const size_t left = node->left == NULL ? 0 : node_height(node->left) + 1;
    const size_t right = node->right == NULL ? 0 : node_height(node->right) + 1;

    return left > right ? left : right;
}

bool int_binary_tree_height(const IntBinaryTree *tree, size_t *out_height) {
    if (tree == NULL || tree->root == NULL || out_height == NULL) return false;

    *out_height = node_height(tree->root);
    return true;
}

static bool validate_node(
    const IntBinaryNode *node,
    const IntBinaryNode *expected_parent,
    size_t limit,
    size_t *count
) {
    if (node == NULL) return true;
    if (node->parent != expected_parent) return false;
    if (node->left != NULL && node->left == node->right) return false;

    ++*count;
    if (*count > limit) return false;

    return validate_node(node->left, node, limit, count) &&
           validate_node(node->right, node, limit, count);
}

bool int_binary_tree_validate(const IntBinaryTree *tree) {
    if (tree == NULL) return false;

    if (tree->size == 0) return tree->root == NULL;
    if (tree->root == NULL || tree->root->parent != NULL) return false;

    size_t count = 0;

    if (!validate_node(tree->root, NULL, tree->size, &count)) return false;

    return count == tree->size;
}
