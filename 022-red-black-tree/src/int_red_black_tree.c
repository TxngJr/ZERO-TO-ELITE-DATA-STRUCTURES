#include "int_red_black_tree.h"

#include <limits.h>
#include <stdlib.h>

typedef enum {
    RB_RED = 0,
    RB_BLACK = 1
} RBColor;

typedef struct RBNode {
    int key;
    RBColor color;
    struct RBNode *parent;
    struct RBNode *left;
    struct RBNode *right;
} RBNode;

struct IntRedBlackTree {
    RBNode *root;
    size_t size;
};

static RBColor color_of(const RBNode *node) {
    return node == NULL ? RB_BLACK : node->color;
}

static RBNode *make_node(int key) {
    RBNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->key = key;
    node->color = RB_RED;
    return node;
}

static void free_subtree(RBNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

IntRedBlackTree *int_rbt_create(void) {
    return calloc(1, sizeof(IntRedBlackTree));
}

void int_rbt_free(IntRedBlackTree *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t int_rbt_size(const IntRedBlackTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

static RBNode *find_node(const IntRedBlackTree *tree, int key) {
    if (tree == NULL) return NULL;

    RBNode *node = tree->root;

    while (node != NULL) {
        if (key == node->key) return node;
        node = key < node->key ? node->left : node->right;
    }

    return NULL;
}

bool int_rbt_contains(const IntRedBlackTree *tree, int key) {
    return find_node(tree, key) != NULL;
}

static void rotate_left(IntRedBlackTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;

    if (y->left != NULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

static void rotate_right(IntRedBlackTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;

    if (x->right != NULL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

static void insert_fixup(IntRedBlackTree *tree, RBNode *node) {
    while (node != tree->root && color_of(node->parent) == RB_RED) {
        RBNode *parent = node->parent;
        RBNode *grand = parent->parent;

        if (parent == grand->left) {
            RBNode *uncle = grand->right;

            if (color_of(uncle) == RB_RED) {
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                grand->color = RB_RED;
                node = grand;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rotate_left(tree, node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = RB_BLACK;
                grand->color = RB_RED;
                rotate_right(tree, grand);
            }
        } else {
            RBNode *uncle = grand->left;

            if (color_of(uncle) == RB_RED) {
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                grand->color = RB_RED;
                node = grand;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotate_right(tree, node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = RB_BLACK;
                grand->color = RB_RED;
                rotate_left(tree, grand);
            }
        }
    }

    if (tree->root != NULL) {
        tree->root->color = RB_BLACK;
    }
}

bool int_rbt_insert(IntRedBlackTree *tree, int key) {
    if (tree == NULL) return false;

    RBNode *parent = NULL;
    RBNode *cursor = tree->root;

    while (cursor != NULL) {
        parent = cursor;

        if (key == cursor->key) return false;

        cursor = key < cursor->key ? cursor->left : cursor->right;
    }

    RBNode *node = make_node(key);
    if (node == NULL) return false;

    node->parent = parent;

    if (parent == NULL) {
        tree->root = node;
    } else if (key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    ++tree->size;
    insert_fixup(tree, node);
    return true;
}

static RBNode *minimum_node(RBNode *node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }

    return node;
}

static void transplant(IntRedBlackTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}

static void delete_fixup(
    IntRedBlackTree *tree,
    RBNode *node,
    RBNode *parent
) {
    while (node != tree->root && color_of(node) == RB_BLACK) {
        if (parent == NULL) break;

        if (node == parent->left) {
            RBNode *sibling = parent->right;

            if (color_of(sibling) == RB_RED) {
                sibling->color = RB_BLACK;
                parent->color = RB_RED;
                rotate_left(tree, parent);
                sibling = parent->right;
            }

            const RBColor left_color =
                color_of(sibling == NULL ? NULL : sibling->left);
            const RBColor right_color =
                color_of(sibling == NULL ? NULL : sibling->right);

            if (left_color == RB_BLACK && right_color == RB_BLACK) {
                if (sibling != NULL) sibling->color = RB_RED;

                node = parent;
                parent = node->parent;
            } else {
                if (right_color == RB_BLACK) {
                    if (sibling != NULL && sibling->left != NULL) {
                        sibling->left->color = RB_BLACK;
                    }

                    if (sibling != NULL) {
                        sibling->color = RB_RED;
                        rotate_right(tree, sibling);
                    }

                    sibling = parent->right;
                }

                if (sibling != NULL) {
                    sibling->color = parent->color;
                }

                parent->color = RB_BLACK;

                if (sibling != NULL && sibling->right != NULL) {
                    sibling->right->color = RB_BLACK;
                }

                rotate_left(tree, parent);
                node = tree->root;
                parent = NULL;
            }
        } else {
            RBNode *sibling = parent->left;

            if (color_of(sibling) == RB_RED) {
                sibling->color = RB_BLACK;
                parent->color = RB_RED;
                rotate_right(tree, parent);
                sibling = parent->left;
            }

            const RBColor left_color =
                color_of(sibling == NULL ? NULL : sibling->left);
            const RBColor right_color =
                color_of(sibling == NULL ? NULL : sibling->right);

            if (left_color == RB_BLACK && right_color == RB_BLACK) {
                if (sibling != NULL) sibling->color = RB_RED;

                node = parent;
                parent = node->parent;
            } else {
                if (left_color == RB_BLACK) {
                    if (sibling != NULL && sibling->right != NULL) {
                        sibling->right->color = RB_BLACK;
                    }

                    if (sibling != NULL) {
                        sibling->color = RB_RED;
                        rotate_left(tree, sibling);
                    }

                    sibling = parent->left;
                }

                if (sibling != NULL) {
                    sibling->color = parent->color;
                }

                parent->color = RB_BLACK;

                if (sibling != NULL && sibling->left != NULL) {
                    sibling->left->color = RB_BLACK;
                }

                rotate_right(tree, parent);
                node = tree->root;
                parent = NULL;
            }
        }
    }

    if (node != NULL) {
        node->color = RB_BLACK;
    }
}

bool int_rbt_remove(IntRedBlackTree *tree, int key) {
    if (tree == NULL) return false;

    RBNode *z = find_node(tree, key);
    if (z == NULL) return false;

    RBNode *y = z;
    RBColor original_color = y->color;
    RBNode *x = NULL;
    RBNode *x_parent = NULL;

    if (z->left == NULL) {
        x = z->right;
        x_parent = z->parent;
        transplant(tree, z, z->right);

        if (x != NULL) x_parent = x->parent;
    } else if (z->right == NULL) {
        x = z->left;
        x_parent = z->parent;
        transplant(tree, z, z->left);

        if (x != NULL) x_parent = x->parent;
    } else {
        y = minimum_node(z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x_parent = y;
            if (x != NULL) x->parent = y;
        } else {
            x_parent = y->parent;
            transplant(tree, y, y->right);

            y->right = z->right;
            y->right->parent = y;

            if (x != NULL) x_parent = x->parent;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);
    --tree->size;

    if (original_color == RB_BLACK) {
        delete_fixup(tree, x, x_parent);
    }

    if (tree->root != NULL) {
        tree->root->color = RB_BLACK;
        tree->root->parent = NULL;
    }

    return true;
}

static size_t node_height(const RBNode *node) {
    if (node == NULL) return 0;

    const size_t left =
        node->left == NULL ? 0 : node_height(node->left) + 1;
    const size_t right =
        node->right == NULL ? 0 : node_height(node->right) + 1;

    return left > right ? left : right;
}

bool int_rbt_height(
    const IntRedBlackTree *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL || out_edge_height == NULL) {
        return false;
    }

    *out_edge_height = node_height(tree->root);
    return true;
}

static void inorder_node(
    const RBNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;

    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool int_rbt_inorder(
    const IntRedBlackTree *tree,
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

static bool validate_node(
    const RBNode *node,
    const RBNode *expected_parent,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t limit,
    size_t *count,
    int *out_black_height
) {
    if (node == NULL) {
        *out_black_height = 1;
        return true;
    }

    if (node->parent != expected_parent) return false;
    if (has_low && node->key <= low) return false;
    if (has_high && node->key >= high) return false;

    if (node->color == RB_RED) {
        if (color_of(node->left) != RB_BLACK) return false;
        if (color_of(node->right) != RB_BLACK) return false;
    }

    ++*count;
    if (*count > limit) return false;

    int left_black_height = 0;
    int right_black_height = 0;

    if (!validate_node(
            node->left,
            node,
            has_low,
            low,
            true,
            node->key,
            limit,
            count,
            &left_black_height
        )) {
        return false;
    }

    if (!validate_node(
            node->right,
            node,
            true,
            node->key,
            has_high,
            high,
            limit,
            count,
            &right_black_height
        )) {
        return false;
    }

    if (left_black_height != right_black_height) return false;

    *out_black_height =
        left_black_height + (node->color == RB_BLACK ? 1 : 0);

    return true;
}

bool int_rbt_validate(const IntRedBlackTree *tree) {
    if (tree == NULL) return false;

    if (tree->size == 0) {
        return tree->root == NULL;
    }

    if (tree->root == NULL) return false;
    if (tree->root->parent != NULL) return false;
    if (tree->root->color != RB_BLACK) return false;

    size_t count = 0;
    int black_height = 0;

    if (!validate_node(
            tree->root,
            NULL,
            false,
            INT_MIN,
            false,
            INT_MAX,
            tree->size,
            &count,
            &black_height
        )) {
        return false;
    }

    return count == tree->size && black_height > 0;
}
