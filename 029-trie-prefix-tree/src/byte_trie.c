#include "byte_trie.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode TrieNode;

typedef struct TrieEdge {
    unsigned char label;
    TrieNode *child;
    struct TrieEdge *next;
} TrieEdge;

struct TrieNode {
    bool terminal;
    TrieEdge *edges;
};

struct ByteTrie {
    TrieNode *root;
    size_t size;
};

static TrieNode *make_node(void) {
    return calloc(1, sizeof(TrieNode));
}

static void free_node(TrieNode *node) {
    if (node == NULL) return;

    TrieEdge *edge = node->edges;

    while (edge != NULL) {
        TrieEdge *next = edge->next;
        free_node(edge->child);
        free(edge);
        edge = next;
    }

    free(node);
}

ByteTrie *byte_trie_create(void) {
    ByteTrie *trie = calloc(1, sizeof *trie);
    if (trie == NULL) return NULL;

    trie->root = make_node();
    if (trie->root == NULL) {
        free(trie);
        return NULL;
    }

    return trie;
}

void byte_trie_free(ByteTrie *trie) {
    if (trie == NULL) return;
    free_node(trie->root);
    free(trie);
}

size_t byte_trie_size(const ByteTrie *trie) {
    return trie == NULL ? 0 : trie->size;
}

static TrieEdge **find_edge_slot(
    TrieNode *node,
    unsigned char label
) {
    TrieEdge **slot = &node->edges;

    while (*slot != NULL && (*slot)->label < label) {
        slot = &(*slot)->next;
    }

    return slot;
}

static const TrieEdge *find_edge_const(
    const TrieNode *node,
    unsigned char label
) {
    const TrieEdge *edge = node->edges;

    while (edge != NULL && edge->label < label) {
        edge = edge->next;
    }

    if (edge != NULL && edge->label == label) return edge;
    return NULL;
}

bool byte_trie_insert(
    ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    if (trie == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    TrieNode *node = trie->root;

    for (size_t i = 0; i < length; ++i) {
        TrieEdge **slot = find_edge_slot(node, bytes[i]);

        if (*slot == NULL || (*slot)->label != bytes[i]) {
            TrieNode *child = make_node();
            TrieEdge *edge = calloc(1, sizeof *edge);

            if (child == NULL || edge == NULL) {
                free(child);
                free(edge);
                return false;
            }

            edge->label = bytes[i];
            edge->child = child;
            edge->next = *slot;
            *slot = edge;
        }

        node = (*slot)->child;
    }

    if (node->terminal) return false;

    node->terminal = true;
    ++trie->size;
    return true;
}

static const TrieNode *find_node_const(
    const ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    if (trie == NULL) return NULL;
    if (length > 0 && bytes == NULL) return NULL;

    const TrieNode *node = trie->root;

    for (size_t i = 0; i < length; ++i) {
        const TrieEdge *edge = find_edge_const(node, bytes[i]);
        if (edge == NULL) return NULL;
        node = edge->child;
    }

    return node;
}

bool byte_trie_contains(
    const ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    const TrieNode *node = find_node_const(trie, bytes, length);
    return node != NULL && node->terminal;
}

bool byte_trie_has_prefix(
    const ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    return find_node_const(trie, bytes, length) != NULL;
}

static size_t count_terminals(const TrieNode *node) {
    if (node == NULL) return 0;

    size_t count = node->terminal ? 1 : 0;

    for (const TrieEdge *edge = node->edges;
         edge != NULL;
         edge = edge->next) {
        count += count_terminals(edge->child);
    }

    return count;
}

size_t byte_trie_count_prefix(
    const ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    const TrieNode *node = find_node_const(trie, bytes, length);
    return node == NULL ? 0 : count_terminals(node);
}

static bool node_is_empty(const TrieNode *node) {
    return node != NULL && !node->terminal && node->edges == NULL;
}

static bool remove_rec(
    TrieNode *node,
    const unsigned char *bytes,
    size_t index,
    size_t length,
    bool *removed
) {
    if (index == length) {
        if (!node->terminal) return node_is_empty(node);

        node->terminal = false;
        *removed = true;
        return node_is_empty(node);
    }

    TrieEdge **slot = find_edge_slot(node, bytes[index]);

    if (*slot == NULL || (*slot)->label != bytes[index]) {
        return node_is_empty(node);
    }

    TrieEdge *edge = *slot;
    const bool prune_child = remove_rec(
        edge->child,
        bytes,
        index + 1,
        length,
        removed
    );

    if (prune_child && *removed) {
        *slot = edge->next;
        free(edge->child);
        free(edge);
    }

    return node_is_empty(node);
}

bool byte_trie_remove(
    ByteTrie *trie,
    const unsigned char *bytes,
    size_t length
) {
    if (trie == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    bool removed = false;
    (void)remove_rec(trie->root, bytes, 0, length, &removed);

    if (removed) --trie->size;
    return removed;
}

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t capacity;
    byte_trie_visit_fn visitor;
    void *context;
    bool ok;
} VisitState;

static bool ensure_buffer(VisitState *state, size_t needed) {
    if (needed <= state->capacity) return true;

    size_t next_capacity = state->capacity == 0 ? 16 : state->capacity;

    while (next_capacity < needed) {
        if (next_capacity > SIZE_MAX / 2) {
            next_capacity = needed;
            break;
        }
        next_capacity *= 2;
    }

    unsigned char *next = realloc(state->buffer, next_capacity);
    if (next == NULL) return false;

    state->buffer = next;
    state->capacity = next_capacity;
    return true;
}

static void visit_rec(const TrieNode *node, VisitState *state) {
    if (!state->ok) return;

    if (node->terminal) {
        if (!state->visitor(state->buffer, state->length, state->context)) {
            state->ok = false;
            return;
        }
    }

    for (const TrieEdge *edge = node->edges;
         edge != NULL && state->ok;
         edge = edge->next) {
        if (!ensure_buffer(state, state->length + 1)) {
            state->ok = false;
            return;
        }

        state->buffer[state->length++] = edge->label;
        visit_rec(edge->child, state);
        --state->length;
    }
}

bool byte_trie_visit_prefix(
    const ByteTrie *trie,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_trie_visit_fn visitor,
    void *context
) {
    if (trie == NULL || visitor == NULL) return false;
    if (prefix_length > 0 && prefix == NULL) return false;

    const TrieNode *node = find_node_const(trie, prefix, prefix_length);
    if (node == NULL) return true;

    VisitState state = {
        .buffer = NULL,
        .length = prefix_length,
        .capacity = 0,
        .visitor = visitor,
        .context = context,
        .ok = true
    };

    if (!ensure_buffer(&state, prefix_length == 0 ? 1 : prefix_length)) {
        return false;
    }

    if (prefix_length > 0) {
        memcpy(state.buffer, prefix, prefix_length);
    }

    visit_rec(node, &state);
    free(state.buffer);

    return state.ok;
}

static bool validate_node(
    const TrieNode *node,
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

    for (const TrieEdge *edge = node->edges;
         edge != NULL;
         edge = edge->next) {
        if (edge->child == NULL) return false;

        if (!first && edge->label <= previous) return false;

        if (!validate_node(edge->child, terminal_limit, terminal_count)) {
            return false;
        }

        previous = edge->label;
        first = false;
    }

    return true;
}

bool byte_trie_validate(const ByteTrie *trie) {
    if (trie == NULL || trie->root == NULL) return false;

    size_t terminal_count = 0;

    if (!validate_node(trie->root, trie->size, &terminal_count)) {
        return false;
    }

    return terminal_count == trie->size;
}
