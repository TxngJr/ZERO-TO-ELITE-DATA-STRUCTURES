#include "tree_traversal.h"

#include <stdint.h>
#include <stdlib.h>

static bool prepare(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written,
    size_t *out_size
) {
    if (tree == NULL || out_written == NULL || out_size == NULL) return false;

    const size_t size = int_binary_tree_size(tree);

    if (size > 0 && (output == NULL || capacity < size)) return false;

    *out_written = 0;
    *out_size = size;
    return true;
}

static void preorder_recursive_node(
    const IntBinaryNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;

    output[(*index)++] = int_binary_node_value(node);
    preorder_recursive_node(int_binary_node_left(node), output, index);
    preorder_recursive_node(int_binary_node_right(node), output, index);
}

static void inorder_recursive_node(
    const IntBinaryNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;

    inorder_recursive_node(int_binary_node_left(node), output, index);
    output[(*index)++] = int_binary_node_value(node);
    inorder_recursive_node(int_binary_node_right(node), output, index);
}

static void postorder_recursive_node(
    const IntBinaryNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;

    postorder_recursive_node(int_binary_node_left(node), output, index);
    postorder_recursive_node(int_binary_node_right(node), output, index);
    output[(*index)++] = int_binary_node_value(node);
}

bool tree_preorder_recursive(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    preorder_recursive_node(int_binary_tree_root(tree), output, out_written);
    return true;
}

bool tree_inorder_recursive(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    inorder_recursive_node(int_binary_tree_root(tree), output, out_written);
    return true;
}

bool tree_postorder_recursive(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    postorder_recursive_node(int_binary_tree_root(tree), output, out_written);
    return true;
}

static IntBinaryNode **allocate_nodes(size_t size) {
    if (size == 0 || size > SIZE_MAX / sizeof(IntBinaryNode *)) return NULL;
    return malloc(size * sizeof(IntBinaryNode *));
}

bool tree_preorder_iterative(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    IntBinaryNode **stack = allocate_nodes(size);
    if (stack == NULL) return false;

    size_t top = 0;
    stack[top++] = int_binary_tree_root(tree);

    while (top > 0) {
        IntBinaryNode *node = stack[--top];
        output[(*out_written)++] = int_binary_node_value(node);

        IntBinaryNode *right = int_binary_node_right(node);
        IntBinaryNode *left = int_binary_node_left(node);

        if (right != NULL) stack[top++] = right;
        if (left != NULL) stack[top++] = left;
    }

    free(stack);
    return true;
}

bool tree_inorder_iterative(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    IntBinaryNode **stack = allocate_nodes(size);
    if (stack == NULL) return false;

    size_t top = 0;
    IntBinaryNode *cursor = int_binary_tree_root(tree);

    while (cursor != NULL || top > 0) {
        while (cursor != NULL) {
            stack[top++] = cursor;
            cursor = int_binary_node_left(cursor);
        }

        cursor = stack[--top];
        output[(*out_written)++] = int_binary_node_value(cursor);
        cursor = int_binary_node_right(cursor);
    }

    free(stack);
    return true;
}

bool tree_postorder_iterative(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    IntBinaryNode **first = allocate_nodes(size);
    IntBinaryNode **second = allocate_nodes(size);

    if (first == NULL || second == NULL) {
        free(first);
        free(second);
        return false;
    }

    size_t first_top = 0;
    size_t second_top = 0;

    first[first_top++] = int_binary_tree_root(tree);

    while (first_top > 0) {
        IntBinaryNode *node = first[--first_top];
        second[second_top++] = node;

        IntBinaryNode *left = int_binary_node_left(node);
        IntBinaryNode *right = int_binary_node_right(node);

        if (left != NULL) first[first_top++] = left;
        if (right != NULL) first[first_top++] = right;
    }

    while (second_top > 0) {
        IntBinaryNode *node = second[--second_top];
        output[(*out_written)++] = int_binary_node_value(node);
    }

    free(first);
    free(second);
    return true;
}

bool tree_level_order(
    const IntBinaryTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    size_t size = 0;
    if (!prepare(tree, output, capacity, out_written, &size)) return false;
    if (size == 0) return true;

    IntBinaryNode **queue = allocate_nodes(size);
    if (queue == NULL) return false;

    size_t front = 0;
    size_t back = 0;

    queue[back++] = int_binary_tree_root(tree);

    while (front < back) {
        IntBinaryNode *node = queue[front++];
        output[(*out_written)++] = int_binary_node_value(node);

        IntBinaryNode *left = int_binary_node_left(node);
        IntBinaryNode *right = int_binary_node_right(node);

        if (left != NULL) queue[back++] = left;
        if (right != NULL) queue[back++] = right;
    }

    free(queue);
    return true;
}
