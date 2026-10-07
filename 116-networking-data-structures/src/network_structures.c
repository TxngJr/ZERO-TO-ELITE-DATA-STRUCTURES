#include "network_structures.h"
#include <stdlib.h>

enum { NONE = SIZE_MAX };

typedef struct {
    size_t child[2];
    bool has_route;
    uint32_t next_hop;
} RouteNode;

struct RouteTrie {
    RouteNode *nodes;
    size_t count;
    size_t capacity;
};

static void route_node_init(RouteNode *node) {
    node->child[0] = NONE;
    node->child[1] = NONE;
    node->has_route = false;
    node->next_hop = 0U;
}

RouteTrie *rt_create(void) {
    RouteTrie *trie = calloc(1, sizeof(*trie));
    if (!trie) return NULL;

    trie->capacity = 16U;
    trie->nodes = malloc(trie->capacity * sizeof(*trie->nodes));
    if (!trie->nodes) {
        free(trie);
        return NULL;
    }

    route_node_init(&trie->nodes[0]);
    trie->count = 1U;
    return trie;
}

void rt_free(RouteTrie *trie) {
    if (!trie) return;
    free(trie->nodes);
    free(trie);
}

static bool rt_grow(RouteTrie *trie) {
    if (trie->capacity > SIZE_MAX / 2U ||
        trie->capacity * 2U > SIZE_MAX / sizeof(*trie->nodes)) {
        return false;
    }

    size_t new_capacity = trie->capacity * 2U;
    RouteNode *grown = realloc(trie->nodes,
                               new_capacity * sizeof(*grown));
    if (!grown) return false;

    trie->nodes = grown;
    trie->capacity = new_capacity;
    return true;
}

static bool rt_new_node(RouteTrie *trie, size_t *out_index) {
    if (trie->count == trie->capacity && !rt_grow(trie)) return false;
    size_t index = trie->count++;
    route_node_init(&trie->nodes[index]);
    *out_index = index;
    return true;
}

bool rt_add(RouteTrie *trie, uint32_t prefix, uint8_t prefix_len,
            uint32_t next_hop) {
    if (!trie || prefix_len > 32U) return false;

    size_t node = 0U;
    for (uint8_t depth = 0U; depth < prefix_len; ++depth) {
        unsigned bit = (unsigned)((prefix >> (31U - depth)) & 1U);
        if (trie->nodes[node].child[bit] == NONE) {
            size_t child = 0U;
            if (!rt_new_node(trie, &child)) return false;
            trie->nodes[node].child[bit] = child;
        }
        node = trie->nodes[node].child[bit];
    }

    trie->nodes[node].has_route = true;
    trie->nodes[node].next_hop = next_hop;
    return true;
}

bool rt_remove(RouteTrie *trie, uint32_t prefix, uint8_t prefix_len,
               bool *out_removed) {
    if (!trie || prefix_len > 32U) return false;

    size_t node = 0U;
    for (uint8_t depth = 0U; depth < prefix_len; ++depth) {
        unsigned bit = (unsigned)((prefix >> (31U - depth)) & 1U);
        size_t child = trie->nodes[node].child[bit];
        if (child == NONE) {
            if (out_removed) *out_removed = false;
            return true;
        }
        node = child;
    }

    bool removed = trie->nodes[node].has_route;
    trie->nodes[node].has_route = false;
    if (out_removed) *out_removed = removed;
    return true;
}

bool rt_lookup(const RouteTrie *trie, uint32_t address,
               uint32_t *out_next_hop, uint8_t *out_prefix_len,
               bool *out_found) {
    if (!trie || !out_next_hop || !out_prefix_len || !out_found)
        return false;

    size_t node = 0U;
    bool found = trie->nodes[0].has_route;
    uint32_t next_hop = trie->nodes[0].next_hop;
    uint8_t matched_len = 0U;

    for (uint8_t depth = 0U; depth < 32U; ++depth) {
        unsigned bit = (unsigned)((address >> (31U - depth)) & 1U);
        size_t child = trie->nodes[node].child[bit];
        if (child == NONE) break;
        node = child;

        if (trie->nodes[node].has_route) {
            found = true;
            next_hop = trie->nodes[node].next_hop;
            matched_len = (uint8_t)(depth + 1U);
        }
    }

    *out_found = found;
    if (found) {
        *out_next_hop = next_hop;
        *out_prefix_len = matched_len;
    }
    return true;
}

size_t rt_node_count(const RouteTrie *trie) {
    return trie ? trie->count : 0U;
}

bool rt_validate(const RouteTrie *trie) {
    if (!trie || !trie->nodes || trie->count == 0U ||
        trie->count > trie->capacity) {
        return false;
    }

    unsigned char *seen = calloc(trie->count, 1U);
    if (!seen) return false;

    size_t *stack = malloc(trie->count * sizeof(*stack));
    if (!stack) {
        free(seen);
        return false;
    }

    size_t top = 0U;
    stack[top++] = 0U;
    size_t visited = 0U;

    while (top) {
        size_t node = stack[--top];
        if (node >= trie->count || seen[node]) {
            free(stack);
            free(seen);
            return false;
        }

        seen[node] = 1U;
        ++visited;
        for (unsigned bit = 0U; bit < 2U; ++bit) {
            size_t child = trie->nodes[node].child[bit];
            if (child != NONE) {
                if (child >= trie->count || top >= trie->count) {
                    free(stack);
                    free(seen);
                    return false;
                }
                stack[top++] = child;
            }
        }
    }

    bool ok = visited == trie->count;
    free(stack);
    free(seen);
    return ok;
}

typedef enum { FLOW_EMPTY=0, FLOW_USED, FLOW_TOMBSTONE } FlowState;
typedef struct {
    FlowKey key;
    uint64_t value;
    FlowState state;
} FlowEntry;

struct FlowTable {
    FlowEntry *entries;
    size_t capacity;
    size_t size;
    size_t tombstones;
};

static uint64_t mix64(uint64_t x) {
    x ^= x >> 30U;
    x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27U;
    x *= UINT64_C(0x94d049bb133111eb);
    x ^= x >> 31U;
    return x;
}

static bool key_equal(FlowKey a, FlowKey b) {
    return a.src_ip == b.src_ip && a.dst_ip == b.dst_ip &&
           a.src_port == b.src_port && a.dst_port == b.dst_port &&
           a.protocol == b.protocol;
}

static uint64_t key_hash(FlowKey key) {
    uint64_t x = ((uint64_t)key.src_ip << 32U) | key.dst_ip;
    x ^= ((uint64_t)key.src_port << 48U) |
         ((uint64_t)key.dst_port << 32U) |
         ((uint64_t)key.protocol << 24U);
    return mix64(x);
}

static bool power_two_at_least(size_t requested, size_t *out) {
    size_t cap = 8U;
    while (cap < requested) {
        if (cap > SIZE_MAX / 2U) return false;
        cap *= 2U;
    }
    *out = cap;
    return true;
}

FlowTable *flow_create(size_t initial_capacity) {
    size_t capacity = 0U;
    if (!power_two_at_least(initial_capacity ? initial_capacity : 8U,
                            &capacity)) {
        return NULL;
    }
    if (capacity > SIZE_MAX / sizeof(FlowEntry)) return NULL;

    FlowTable *table = calloc(1, sizeof(*table));
    if (!table) return NULL;
    table->entries = calloc(capacity, sizeof(*table->entries));
    if (!table->entries) {
        free(table);
        return NULL;
    }
    table->capacity = capacity;
    return table;
}

void flow_free(FlowTable *table) {
    if (!table) return;
    free(table->entries);
    free(table);
}

static bool flow_insert_raw(FlowTable *table, FlowKey key, uint64_t value) {
    size_t mask = table->capacity - 1U;
    size_t index = (size_t)key_hash(key) & mask;
    while (table->entries[index].state == FLOW_USED)
        index = (index + 1U) & mask;

    table->entries[index].key = key;
    table->entries[index].value = value;
    table->entries[index].state = FLOW_USED;
    ++table->size;
    return true;
}

static bool flow_rehash(FlowTable *table, size_t new_capacity) {
    if (new_capacity > SIZE_MAX / sizeof(FlowEntry)) return false;
    FlowEntry *old_entries = table->entries;
    size_t old_capacity = table->capacity;

    FlowEntry *new_entries = calloc(new_capacity, sizeof(*new_entries));
    if (!new_entries) return false;

    table->entries = new_entries;
    table->capacity = new_capacity;
    table->size = 0U;
    table->tombstones = 0U;

    for (size_t i = 0; i < old_capacity; ++i) {
        if (old_entries[i].state == FLOW_USED)
            flow_insert_raw(table, old_entries[i].key, old_entries[i].value);
    }
    free(old_entries);
    return true;
}

bool flow_put(FlowTable *table, FlowKey key, uint64_t value,
              bool *out_inserted) {
    if (!table) return false;

    if ((table->size + table->tombstones + 1U) * 10U >
        table->capacity * 7U) {
        if (table->capacity > SIZE_MAX / 2U) return false;
        if (!flow_rehash(table, table->capacity * 2U)) return false;
    }

    size_t mask = table->capacity - 1U;
    size_t index = (size_t)key_hash(key) & mask;
    size_t first_tombstone = NONE;

    for (;;) {
        FlowEntry *entry = &table->entries[index];
        if (entry->state == FLOW_EMPTY) {
            size_t target = first_tombstone != NONE ? first_tombstone : index;
            FlowEntry *slot = &table->entries[target];
            if (slot->state == FLOW_TOMBSTONE) --table->tombstones;
            slot->key = key;
            slot->value = value;
            slot->state = FLOW_USED;
            ++table->size;
            if (out_inserted) *out_inserted = true;
            return true;
        }

        if (entry->state == FLOW_TOMBSTONE) {
            if (first_tombstone == NONE) first_tombstone = index;
        } else if (key_equal(entry->key, key)) {
            entry->value = value;
            if (out_inserted) *out_inserted = false;
            return true;
        }

        index = (index + 1U) & mask;
    }
}

bool flow_get(const FlowTable *table, FlowKey key, uint64_t *out_value,
              bool *out_found) {
    if (!table || !out_value || !out_found) return false;

    size_t mask = table->capacity - 1U;
    size_t index = (size_t)key_hash(key) & mask;

    for (size_t probes = 0; probes < table->capacity; ++probes) {
        const FlowEntry *entry = &table->entries[index];
        if (entry->state == FLOW_EMPTY) {
            *out_found = false;
            return true;
        }
        if (entry->state == FLOW_USED && key_equal(entry->key, key)) {
            *out_value = entry->value;
            *out_found = true;
            return true;
        }
        index = (index + 1U) & mask;
    }

    *out_found = false;
    return true;
}

bool flow_remove(FlowTable *table, FlowKey key, bool *out_removed) {
    if (!table) return false;

    size_t mask = table->capacity - 1U;
    size_t index = (size_t)key_hash(key) & mask;
    for (size_t probes = 0; probes < table->capacity; ++probes) {
        FlowEntry *entry = &table->entries[index];
        if (entry->state == FLOW_EMPTY) {
            if (out_removed) *out_removed = false;
            return true;
        }
        if (entry->state == FLOW_USED && key_equal(entry->key, key)) {
            entry->state = FLOW_TOMBSTONE;
            --table->size;
            ++table->tombstones;
            if (out_removed) *out_removed = true;
            return true;
        }
        index = (index + 1U) & mask;
    }

    if (out_removed) *out_removed = false;
    return true;
}

size_t flow_size(const FlowTable *table) {
    return table ? table->size : 0U;
}

size_t flow_capacity(const FlowTable *table) {
    return table ? table->capacity : 0U;
}

bool flow_validate(const FlowTable *table) {
    if (!table || !table->entries || table->capacity < 8U ||
        (table->capacity & (table->capacity - 1U)) != 0U) {
        return false;
    }

    size_t used = 0U;
    size_t tombstones = 0U;
    for (size_t i = 0; i < table->capacity; ++i) {
        const FlowEntry *entry = &table->entries[i];
        if (entry->state == FLOW_USED) {
            ++used;
            uint64_t value = 0U;
            bool found = false;
            if (!flow_get(table, entry->key, &value, &found) || !found ||
                value != entry->value) {
                return false;
            }
        } else if (entry->state == FLOW_TOMBSTONE) {
            ++tombstones;
        } else if (entry->state != FLOW_EMPTY) {
            return false;
        }
    }

    return used == table->size && tombstones == table->tombstones &&
           used + tombstones <= table->capacity;
}
