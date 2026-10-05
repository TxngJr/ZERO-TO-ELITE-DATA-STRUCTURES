#include "byte_radix_tree.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct RadixNode RadixNode;

typedef struct RadixEdge {
    unsigned char *label;
    size_t length;
    RadixNode *child;
    struct RadixEdge *next;
} RadixEdge;

struct RadixNode {
    bool terminal;
    RadixEdge *edges;
};

struct ByteRadixTree {
    RadixNode *root;
    size_t size;
};

static RadixNode *make_node(void) {
    return calloc(1, sizeof(RadixNode));
}

static unsigned char *copy_bytes(
    const unsigned char *bytes,
    size_t length
) {
    if (length == 0) return NULL;

    unsigned char *copy = malloc(length);
    if (copy == NULL) return NULL;

    memcpy(copy, bytes, length);
    return copy;
}

static void free_node(RadixNode *node) {
    if (node == NULL) return;

    RadixEdge *edge = node->edges;

    while (edge != NULL) {
        RadixEdge *next = edge->next;
        free(edge->label);
        free_node(edge->child);
        free(edge);
        edge = next;
    }

    free(node);
}

ByteRadixTree *byte_radix_tree_create(void) {
    ByteRadixTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->root = make_node();

    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }

    return tree;
}

void byte_radix_tree_free(ByteRadixTree *tree) {
    if (tree == NULL) return;
    free_node(tree->root);
    free(tree);
}

size_t byte_radix_tree_size(const ByteRadixTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

static size_t count_nodes(const RadixNode *node) {
    if (node == NULL) return 0;

    size_t count = 1;

    for (const RadixEdge *edge = node->edges;
         edge != NULL;
         edge = edge->next) {
        count += count_nodes(edge->child);
    }

    return count;
}

size_t byte_radix_tree_node_count(const ByteRadixTree *tree) {
    return tree == NULL ? 0 : count_nodes(tree->root);
}

static RadixEdge **find_edge_slot(
    RadixNode *node,
    unsigned char first_byte
) {
    RadixEdge **slot = &node->edges;

    while (*slot != NULL &&
           (*slot)->label[0] < first_byte) {
        slot = &(*slot)->next;
    }

    return slot;
}

static const RadixEdge *find_edge_const(
    const RadixNode *node,
    unsigned char first_byte
) {
    const RadixEdge *edge = node->edges;

    while (edge != NULL &&
           edge->label[0] < first_byte) {
        edge = edge->next;
    }

    if (edge != NULL && edge->label[0] == first_byte) {
        return edge;
    }

    return NULL;
}

static size_t common_prefix_length(
    const unsigned char *a,
    size_t a_length,
    const unsigned char *b,
    size_t b_length
) {
    const size_t limit = a_length < b_length ? a_length : b_length;
    size_t i = 0;

    while (i < limit && a[i] == b[i]) ++i;
    return i;
}

static bool insert_new_suffix(
    RadixEdge **slot,
    const unsigned char *suffix,
    size_t suffix_length
) {
    RadixNode *leaf = make_node();
    RadixEdge *edge = calloc(1, sizeof *edge);
    unsigned char *label = copy_bytes(suffix, suffix_length);

    if (leaf == NULL || edge == NULL || label == NULL) {
        free(leaf);
        free(edge);
        free(label);
        return false;
    }

    leaf->terminal = true;

    edge->label = label;
    edge->length = suffix_length;
    edge->child = leaf;
    edge->next = *slot;
    *slot = edge;

    return true;
}

bool byte_radix_tree_insert(
    ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    if (length == 0) {
        if (tree->root->terminal) return false;

        tree->root->terminal = true;
        ++tree->size;
        return true;
    }

    RadixNode *node = tree->root;
    size_t position = 0;

    while (position < length) {
        RadixEdge **slot = find_edge_slot(node, bytes[position]);
        RadixEdge *edge = *slot;

        if (edge == NULL || edge->label[0] != bytes[position]) {
            if (!insert_new_suffix(
                    slot,
                    bytes + position,
                    length - position
                )) {
                return false;
            }

            ++tree->size;
            return true;
        }

        const size_t remaining = length - position;
        const size_t common = common_prefix_length(
            bytes + position,
            remaining,
            edge->label,
            edge->length
        );

        if (common == edge->length) {
            position += common;
            node = edge->child;
            continue;
        }

        if (common == 0) return false;

        RadixNode *middle = make_node();
        RadixEdge *old_suffix_edge = calloc(1, sizeof *old_suffix_edge);

        unsigned char *prefix_label = copy_bytes(edge->label, common);
        unsigned char *old_suffix_label = copy_bytes(
            edge->label + common,
            edge->length - common
        );

        if (middle == NULL ||
            old_suffix_edge == NULL ||
            prefix_label == NULL ||
            old_suffix_label == NULL) {
            free(middle);
            free(old_suffix_edge);
            free(prefix_label);
            free(old_suffix_label);
            return false;
        }

        RadixEdge *new_suffix_edge = NULL;
        RadixNode *new_leaf = NULL;
        unsigned char *new_suffix_label = NULL;

        const size_t key_suffix_length = remaining - common;

        if (key_suffix_length > 0) {
            new_suffix_edge = calloc(1, sizeof *new_suffix_edge);
            new_leaf = make_node();
            new_suffix_label = copy_bytes(
                bytes + position + common,
                key_suffix_length
            );

            if (new_suffix_edge == NULL ||
                new_leaf == NULL ||
                new_suffix_label == NULL) {
                free(middle);
                free(old_suffix_edge);
                free(prefix_label);
                free(old_suffix_label);
                free(new_suffix_edge);
                free(new_leaf);
                free(new_suffix_label);
                return false;
            }

            new_leaf->terminal = true;
            new_suffix_edge->label = new_suffix_label;
            new_suffix_edge->length = key_suffix_length;
            new_suffix_edge->child = new_leaf;
        } else {
            middle->terminal = true;
        }

        old_suffix_edge->label = old_suffix_label;
        old_suffix_edge->length = edge->length - common;
        old_suffix_edge->child = edge->child;

        if (new_suffix_edge == NULL) {
            middle->edges = old_suffix_edge;
        } else if (old_suffix_edge->label[0] < new_suffix_edge->label[0]) {
            old_suffix_edge->next = new_suffix_edge;
            middle->edges = old_suffix_edge;
        } else {
            new_suffix_edge->next = old_suffix_edge;
            middle->edges = new_suffix_edge;
        }

        free(edge->label);
        edge->label = prefix_label;
        edge->length = common;
        edge->child = middle;

        ++tree->size;
        return true;
    }

    if (node->terminal) return false;

    node->terminal = true;
    ++tree->size;
    return true;
}

static const RadixNode *find_exact_node(
    const ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return NULL;
    if (length > 0 && bytes == NULL) return NULL;

    const RadixNode *node = tree->root;
    size_t position = 0;

    while (position < length) {
        const RadixEdge *edge = find_edge_const(node, bytes[position]);
        if (edge == NULL) return NULL;

        const size_t remaining = length - position;

        if (remaining < edge->length) return NULL;

        if (memcmp(
                bytes + position,
                edge->label,
                edge->length
            ) != 0) {
            return NULL;
        }

        position += edge->length;
        node = edge->child;
    }

    return node;
}

bool byte_radix_tree_contains(
    const ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    const RadixNode *node = find_exact_node(tree, bytes, length);
    return node != NULL && node->terminal;
}

typedef struct {
    const RadixNode *node;
    const unsigned char *extra;
    size_t extra_length;
} PrefixLocation;

static bool locate_prefix(
    const ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length,
    PrefixLocation *out
) {
    if (tree == NULL || out == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    const RadixNode *node = tree->root;
    size_t position = 0;

    if (length == 0) {
        out->node = node;
        out->extra = NULL;
        out->extra_length = 0;
        return true;
    }

    while (position < length) {
        const RadixEdge *edge = find_edge_const(node, bytes[position]);
        if (edge == NULL) return false;

        const size_t remaining = length - position;
        const size_t common = common_prefix_length(
            bytes + position,
            remaining,
            edge->label,
            edge->length
        );

        if (common == remaining) {
            out->node = edge->child;
            out->extra = edge->label + common;
            out->extra_length = edge->length - common;
            return true;
        }

        if (common != edge->length) return false;

        position += edge->length;
        node = edge->child;
    }

    out->node = node;
    out->extra = NULL;
    out->extra_length = 0;
    return true;
}

bool byte_radix_tree_has_prefix(
    const ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    PrefixLocation location;
    return locate_prefix(tree, bytes, length, &location);
}

static size_t count_terminals(const RadixNode *node) {
    if (node == NULL) return 0;

    size_t count = node->terminal ? 1 : 0;

    for (const RadixEdge *edge = node->edges;
         edge != NULL;
         edge = edge->next) {
        count += count_terminals(edge->child);
    }

    return count;
}

size_t byte_radix_tree_count_prefix(
    const ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    PrefixLocation location;

    if (!locate_prefix(tree, bytes, length, &location)) {
        return 0;
    }

    return count_terminals(location.node);
}

static bool node_empty(const RadixNode *node) {
    return node != NULL && !node->terminal && node->edges == NULL;
}

static void try_compress_edge(RadixEdge *edge) {
    RadixNode *child = edge->child;

    if (child == NULL ||
        child->terminal ||
        child->edges == NULL ||
        child->edges->next != NULL) {
        return;
    }

    RadixEdge *only = child->edges;

    if (edge->length > SIZE_MAX - only->length) return;

    const size_t combined_length = edge->length + only->length;
    unsigned char *combined = malloc(combined_length);

    if (combined == NULL) return;

    memcpy(combined, edge->label, edge->length);
    memcpy(
        combined + edge->length,
        only->label,
        only->length
    );

    free(edge->label);
    edge->label = combined;
    edge->length = combined_length;
    edge->child = only->child;

    child->edges = NULL;

    free(only->label);
    free(only);
    free(child);
}

static bool remove_rec(
    RadixNode *node,
    const unsigned char *bytes,
    size_t position,
    size_t length,
    bool *removed
) {
    if (position == length) {
        if (!node->terminal) return node_empty(node);

        node->terminal = false;
        *removed = true;
        return node_empty(node);
    }

    RadixEdge **slot = find_edge_slot(node, bytes[position]);
    RadixEdge *edge = *slot;

    if (edge == NULL || edge->label[0] != bytes[position]) {
        return node_empty(node);
    }

    const size_t remaining = length - position;

    if (remaining < edge->length) return node_empty(node);

    if (memcmp(
            bytes + position,
            edge->label,
            edge->length
        ) != 0) {
        return node_empty(node);
    }

    const bool prune_child = remove_rec(
        edge->child,
        bytes,
        position + edge->length,
        length,
        removed
    );

    if (*removed && prune_child) {
        *slot = edge->next;
        free(edge->label);
        free_node(edge->child);
        free(edge);
    } else if (*removed) {
        try_compress_edge(edge);
    }

    return node_empty(node);
}

bool byte_radix_tree_remove(
    ByteRadixTree *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    bool removed = false;

    (void)remove_rec(
        tree->root,
        bytes,
        0,
        length,
        &removed
    );

    if (removed) --tree->size;
    return removed;
}

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t capacity;
    byte_radix_visit_fn visitor;
    void *context;
    bool ok;
} VisitState;

static bool ensure_buffer(
    VisitState *state,
    size_t needed
) {
    if (needed <= state->capacity) return true;

    size_t capacity = state->capacity == 0 ? 16 : state->capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    unsigned char *next = realloc(state->buffer, capacity);
    if (next == NULL) return false;

    state->buffer = next;
    state->capacity = capacity;
    return true;
}

static void visit_rec(
    const RadixNode *node,
    VisitState *state
) {
    if (!state->ok) return;

    if (node->terminal) {
        if (!state->visitor(
                state->buffer,
                state->length,
                state->context
            )) {
            state->ok = false;
            return;
        }
    }

    for (const RadixEdge *edge = node->edges;
         edge != NULL && state->ok;
         edge = edge->next) {
        if (state->length > SIZE_MAX - edge->length) {
            state->ok = false;
            return;
        }

        const size_t old_length = state->length;
        const size_t new_length = old_length + edge->length;

        if (!ensure_buffer(state, new_length == 0 ? 1 : new_length)) {
            state->ok = false;
            return;
        }

        memcpy(
            state->buffer + old_length,
            edge->label,
            edge->length
        );

        state->length = new_length;
        visit_rec(edge->child, state);
        state->length = old_length;
    }
}

bool byte_radix_tree_visit_prefix(
    const ByteRadixTree *tree,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_radix_visit_fn visitor,
    void *context
) {
    if (tree == NULL || visitor == NULL) return false;
    if (prefix_length > 0 && prefix == NULL) return false;

    PrefixLocation location;

    if (!locate_prefix(tree, prefix, prefix_length, &location)) {
        return true;
    }

    if (prefix_length > SIZE_MAX - location.extra_length) {
        return false;
    }

    VisitState state = {
        .buffer = NULL,
        .length = prefix_length + location.extra_length,
        .capacity = 0,
        .visitor = visitor,
        .context = context,
        .ok = true
    };

    const size_t needed = state.length == 0 ? 1 : state.length;

    if (!ensure_buffer(&state, needed)) return false;

    if (prefix_length > 0) {
        memcpy(state.buffer, prefix, prefix_length);
    }

    if (location.extra_length > 0) {
        memcpy(
            state.buffer + prefix_length,
            location.extra,
            location.extra_length
        );
    }

    visit_rec(location.node, &state);
    free(state.buffer);

    return state.ok;
}

static bool validate_node(
    const RadixNode *node,
    size_t terminal_limit,
    size_t *terminal_count
) {
    if (node == NULL) return false;

    if (node->terminal) {
        if (*terminal_count >= terminal_limit) return false;
        ++*terminal_count;
    }

    bool first = true;
    unsigned char previous = 0;

    for (const RadixEdge *edge = node->edges;
         edge != NULL;
         edge = edge->next) {
        if (edge->length == 0) return false;
        if (edge->label == NULL) return false;
        if (edge->child == NULL) return false;

        if (!first && edge->label[0] <= previous) return false;

        if (!validate_node(
                edge->child,
                terminal_limit,
                terminal_count
            )) {
            return false;
        }

        previous = edge->label[0];
        first = false;
    }

    return true;
}

bool byte_radix_tree_validate(
    const ByteRadixTree *tree
) {
    if (tree == NULL || tree->root == NULL) return false;

    size_t terminal_count = 0;

    if (!validate_node(
            tree->root,
            tree->size,
            &terminal_count
        )) {
        return false;
    }

    return terminal_count == tree->size;
}
