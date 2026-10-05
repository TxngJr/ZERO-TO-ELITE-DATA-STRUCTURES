#include "int_lsm_tree.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int key;
    int value;
    bool tombstone;
    uint64_t sequence;
} LSMEntry;

typedef struct {
    LSMEntry *entries;
    size_t count;
    uint64_t min_sequence;
    uint64_t max_sequence;
} LSMRun;

struct IntLSMTree {
    LSMEntry *memtable;
    size_t memtable_count;
    size_t memtable_capacity;

    LSMRun *runs;
    size_t run_count;
    size_t run_capacity;

    uint64_t next_sequence;
    size_t logical_size;
};

static size_t lower_bound_entries(
    const LSMEntry *entries,
    size_t count,
    int key
) {
    size_t low = 0;
    size_t high = count;

    while (low < high) {
        const size_t mid = low + (high - low) / 2U;

        if (entries[mid].key < key) {
            low = mid + 1U;
        } else {
            high = mid;
        }
    }

    return low;
}

static const LSMEntry *find_entry(
    const LSMEntry *entries,
    size_t count,
    int key
) {
    const size_t index =
        lower_bound_entries(entries, count, key);

    if (index < count && entries[index].key == key) {
        return &entries[index];
    }

    return NULL;
}

IntLSMTree *int_lsm_tree_create(
    size_t memtable_capacity
) {
    if (memtable_capacity == 0) return NULL;

    if (memtable_capacity >
        SIZE_MAX / sizeof(LSMEntry)) {
        return NULL;
    }

    IntLSMTree *tree = calloc(1, sizeof *tree);
    if (tree == NULL) return NULL;

    tree->memtable = malloc(
        memtable_capacity * sizeof *tree->memtable
    );

    if (tree->memtable == NULL) {
        free(tree);
        return NULL;
    }

    tree->memtable_capacity = memtable_capacity;
    tree->next_sequence = 1;
    return tree;
}

void int_lsm_tree_free(IntLSMTree *tree) {
    if (tree == NULL) return;

    for (size_t i = 0; i < tree->run_count; ++i) {
        free(tree->runs[i].entries);
    }

    free(tree->runs);
    free(tree->memtable);
    free(tree);
}

size_t int_lsm_tree_size(const IntLSMTree *tree) {
    return tree == NULL ? 0 : tree->logical_size;
}

size_t int_lsm_tree_memtable_size(
    const IntLSMTree *tree
) {
    return tree == NULL ? 0 : tree->memtable_count;
}

size_t int_lsm_tree_run_count(
    const IntLSMTree *tree
) {
    return tree == NULL ? 0 : tree->run_count;
}

bool int_lsm_tree_get(
    const IntLSMTree *tree,
    int key,
    int *out_value
) {
    if (tree == NULL) return false;

    const LSMEntry *entry = find_entry(
        tree->memtable,
        tree->memtable_count,
        key
    );

    if (entry != NULL) {
        if (entry->tombstone) return false;

        if (out_value != NULL) {
            *out_value = entry->value;
        }
        return true;
    }

    for (size_t i = 0; i < tree->run_count; ++i) {
        entry = find_entry(
            tree->runs[i].entries,
            tree->runs[i].count,
            key
        );

        if (entry == NULL) continue;
        if (entry->tombstone) return false;

        if (out_value != NULL) {
            *out_value = entry->value;
        }
        return true;
    }

    return false;
}

static bool reserve_runs(
    IntLSMTree *tree,
    size_t needed
) {
    if (needed <= tree->run_capacity) return true;

    size_t capacity =
        tree->run_capacity == 0 ? 4 : tree->run_capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2U) {
            capacity = needed;
            break;
        }
        capacity *= 2U;
    }

    if (capacity > SIZE_MAX / sizeof *tree->runs) {
        return false;
    }

    LSMRun *next = realloc(
        tree->runs,
        capacity * sizeof *next
    );

    if (next == NULL) return false;

    tree->runs = next;
    tree->run_capacity = capacity;
    return true;
}

bool int_lsm_tree_flush(IntLSMTree *tree) {
    if (tree == NULL) return false;
    if (tree->memtable_count == 0) return true;

    if (!reserve_runs(tree, tree->run_count + 1U)) {
        return false;
    }

    if (tree->memtable_count >
        SIZE_MAX / sizeof(LSMEntry)) {
        return false;
    }

    LSMEntry *copy = malloc(
        tree->memtable_count * sizeof *copy
    );
    if (copy == NULL) return false;

    memcpy(
        copy,
        tree->memtable,
        tree->memtable_count * sizeof *copy
    );

    uint64_t min_sequence = copy[0].sequence;
    uint64_t max_sequence = copy[0].sequence;

    for (size_t i = 1;
         i < tree->memtable_count;
         ++i) {
        if (copy[i].sequence < min_sequence) {
            min_sequence = copy[i].sequence;
        }
        if (copy[i].sequence > max_sequence) {
            max_sequence = copy[i].sequence;
        }
    }

    for (size_t i = tree->run_count; i > 0; --i) {
        tree->runs[i] = tree->runs[i - 1U];
    }

    tree->runs[0] = (LSMRun){
        .entries = copy,
        .count = tree->memtable_count,
        .min_sequence = min_sequence,
        .max_sequence = max_sequence
    };

    ++tree->run_count;
    tree->memtable_count = 0;
    return true;
}

static bool next_sequence(
    IntLSMTree *tree,
    uint64_t *out_sequence
) {
    if (tree->next_sequence == UINT64_MAX) {
        return false;
    }

    *out_sequence = tree->next_sequence++;
    return true;
}

static bool memtable_upsert(
    IntLSMTree *tree,
    int key,
    int value,
    bool tombstone,
    uint64_t sequence
) {
    size_t index = lower_bound_entries(
        tree->memtable,
        tree->memtable_count,
        key
    );

    if (index < tree->memtable_count &&
        tree->memtable[index].key == key) {
        tree->memtable[index] = (LSMEntry){
            .key = key,
            .value = value,
            .tombstone = tombstone,
            .sequence = sequence
        };
        return true;
    }

    if (tree->memtable_count ==
        tree->memtable_capacity) {
        return false;
    }

    for (size_t i = tree->memtable_count;
         i > index;
         --i) {
        tree->memtable[i] =
            tree->memtable[i - 1U];
    }

    tree->memtable[index] = (LSMEntry){
        .key = key,
        .value = value,
        .tombstone = tombstone,
        .sequence = sequence
    };

    ++tree->memtable_count;
    return true;
}

static bool ensure_room_for_new_mem_key(
    IntLSMTree *tree,
    int key
) {
    const size_t index = lower_bound_entries(
        tree->memtable,
        tree->memtable_count,
        key
    );

    if (index < tree->memtable_count &&
        tree->memtable[index].key == key) {
        return true;
    }

    if (tree->memtable_count <
        tree->memtable_capacity) {
        return true;
    }

    return int_lsm_tree_flush(tree);
}

bool int_lsm_tree_put(
    IntLSMTree *tree,
    int key,
    int value
) {
    if (tree == NULL) return false;

    const bool existed =
        int_lsm_tree_get(tree, key, NULL);

    if (!ensure_room_for_new_mem_key(tree, key)) {
        return false;
    }

    uint64_t sequence = 0;
    if (!next_sequence(tree, &sequence)) return false;

    if (!memtable_upsert(
            tree,
            key,
            value,
            false,
            sequence
        )) {
        return false;
    }

    if (!existed) ++tree->logical_size;
    return true;
}

bool int_lsm_tree_delete(
    IntLSMTree *tree,
    int key
) {
    if (tree == NULL) return false;

    if (!int_lsm_tree_get(tree, key, NULL)) {
        return false;
    }

    if (!ensure_room_for_new_mem_key(tree, key)) {
        return false;
    }

    uint64_t sequence = 0;
    if (!next_sequence(tree, &sequence)) return false;

    if (!memtable_upsert(
            tree,
            key,
            0,
            true,
            sequence
        )) {
        return false;
    }

    --tree->logical_size;
    return true;
}

static int compare_entry_version(
    const void *left_ptr,
    const void *right_ptr
) {
    const LSMEntry *left = left_ptr;
    const LSMEntry *right = right_ptr;

    if (left->key < right->key) return -1;
    if (left->key > right->key) return 1;

    if (left->sequence > right->sequence) return -1;
    if (left->sequence < right->sequence) return 1;

    return 0;
}

static bool gather_versions(
    const IntLSMTree *tree,
    bool include_memtable,
    LSMEntry **out_entries,
    size_t *out_count
) {
    if (out_entries == NULL || out_count == NULL) {
        return false;
    }

    size_t total =
        include_memtable ? tree->memtable_count : 0;

    for (size_t i = 0; i < tree->run_count; ++i) {
        if (total > SIZE_MAX - tree->runs[i].count) {
            return false;
        }
        total += tree->runs[i].count;
    }

    *out_entries = NULL;
    *out_count = total;

    if (total == 0) return true;

    if (total > SIZE_MAX / sizeof(LSMEntry)) {
        return false;
    }

    LSMEntry *entries = malloc(
        total * sizeof *entries
    );
    if (entries == NULL) return false;

    size_t index = 0;

    if (include_memtable) {
        memcpy(
            entries,
            tree->memtable,
            tree->memtable_count * sizeof *entries
        );
        index += tree->memtable_count;
    }

    for (size_t r = 0; r < tree->run_count; ++r) {
        memcpy(
            entries + index,
            tree->runs[r].entries,
            tree->runs[r].count * sizeof *entries
        );
        index += tree->runs[r].count;
    }

    qsort(
        entries,
        total,
        sizeof *entries,
        compare_entry_version
    );

    *out_entries = entries;
    return true;
}

static size_t compact_visible_in_place(
    LSMEntry *entries,
    size_t count,
    bool keep_tombstones
) {
    size_t output = 0;
    size_t index = 0;

    while (index < count) {
        const int key = entries[index].key;
        const LSMEntry newest = entries[index];

        while (index < count &&
               entries[index].key == key) {
            ++index;
        }

        if (!newest.tombstone || keep_tombstones) {
            entries[output++] = newest;
        }
    }

    return output;
}

bool int_lsm_tree_compact(IntLSMTree *tree) {
    if (tree == NULL) return false;
    if (tree->run_count <= 1) return true;

    LSMEntry *entries = NULL;
    size_t count = 0;

    if (!gather_versions(
            tree,
            false,
            &entries,
            &count
        )) {
        return false;
    }

    const size_t live_count =
        compact_visible_in_place(
            entries,
            count,
            false
        );

    uint64_t min_sequence = 0;
    uint64_t max_sequence = 0;

    if (live_count > 0) {
        min_sequence = entries[0].sequence;
        max_sequence = entries[0].sequence;

        for (size_t i = 1; i < live_count; ++i) {
            if (entries[i].sequence < min_sequence) {
                min_sequence = entries[i].sequence;
            }
            if (entries[i].sequence > max_sequence) {
                max_sequence = entries[i].sequence;
            }
        }
    }

    for (size_t i = 0; i < tree->run_count; ++i) {
        free(tree->runs[i].entries);
    }

    if (live_count == 0) {
        free(entries);
        tree->run_count = 0;
        return true;
    }

    tree->runs[0] = (LSMRun){
        .entries = entries,
        .count = live_count,
        .min_sequence = min_sequence,
        .max_sequence = max_sequence
    };
    tree->run_count = 1;

    return true;
}

static bool materialize_visible(
    const IntLSMTree *tree,
    LSMEntry **out_entries,
    size_t *out_count
) {
    LSMEntry *entries = NULL;
    size_t count = 0;

    if (!gather_versions(
            tree,
            true,
            &entries,
            &count
        )) {
        return false;
    }

    const size_t visible =
        compact_visible_in_place(
            entries,
            count,
            false
        );

    *out_entries = entries;
    *out_count = visible;
    return true;
}

bool int_lsm_tree_scan(
    const IntLSMTree *tree,
    int low,
    int high,
    int *keys,
    int *values,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) {
        return false;
    }

    *out_written = 0;

    if (low > high || tree->logical_size == 0) {
        return true;
    }

    LSMEntry *visible = NULL;
    size_t visible_count = 0;

    if (!materialize_visible(
            tree,
            &visible,
            &visible_count
        )) {
        return false;
    }

    size_t needed = 0;

    for (size_t i = 0; i < visible_count; ++i) {
        if (visible[i].key >= low &&
            visible[i].key <= high) {
            ++needed;
        }
    }

    if (needed > capacity ||
        (needed > 0 &&
         (keys == NULL || values == NULL))) {
        free(visible);
        return false;
    }

    size_t output = 0;

    for (size_t i = 0; i < visible_count; ++i) {
        if (visible[i].key < low ||
            visible[i].key > high) {
            continue;
        }

        keys[output] = visible[i].key;
        values[output] = visible[i].value;
        ++output;
    }

    free(visible);
    *out_written = output;
    return true;
}

static bool entries_sorted_unique(
    const LSMEntry *entries,
    size_t count
) {
    for (size_t i = 1; i < count; ++i) {
        if (entries[i - 1U].key >= entries[i].key) {
            return false;
        }
    }

    return true;
}

bool int_lsm_tree_validate(
    const IntLSMTree *tree
) {
    if (tree == NULL ||
        tree->memtable == NULL ||
        tree->memtable_capacity == 0) {
        return false;
    }

    if (tree->memtable_count >
        tree->memtable_capacity) {
        return false;
    }

    if (!entries_sorted_unique(
            tree->memtable,
            tree->memtable_count
        )) {
        return false;
    }

    uint64_t mem_min_sequence = UINT64_MAX;

    for (size_t i = 0;
         i < tree->memtable_count;
         ++i) {
        const uint64_t sequence =
            tree->memtable[i].sequence;

        if (sequence == 0 ||
            sequence >= tree->next_sequence) {
            return false;
        }

        if (sequence < mem_min_sequence) {
            mem_min_sequence = sequence;
        }
    }

    for (size_t r = 0; r < tree->run_count; ++r) {
        const LSMRun *run = &tree->runs[r];

        if (run->count == 0 || run->entries == NULL) {
            return false;
        }

        if (!entries_sorted_unique(
                run->entries,
                run->count
            )) {
            return false;
        }

        uint64_t min_sequence =
            run->entries[0].sequence;
        uint64_t max_sequence =
            run->entries[0].sequence;

        for (size_t i = 0; i < run->count; ++i) {
            const uint64_t sequence =
                run->entries[i].sequence;

            if (sequence == 0 ||
                sequence >= tree->next_sequence) {
                return false;
            }

            if (sequence < min_sequence) {
                min_sequence = sequence;
            }
            if (sequence > max_sequence) {
                max_sequence = sequence;
            }
        }

        if (min_sequence != run->min_sequence ||
            max_sequence != run->max_sequence) {
            return false;
        }

        if (r + 1U < tree->run_count &&
            run->min_sequence <=
                tree->runs[r + 1U].max_sequence) {
            return false;
        }
    }

    if (tree->memtable_count > 0 &&
        tree->run_count > 0 &&
        mem_min_sequence <=
            tree->runs[0].max_sequence) {
        return false;
    }

    LSMEntry *visible = NULL;
    size_t visible_count = 0;

    if (!materialize_visible(
            tree,
            &visible,
            &visible_count
        )) {
        return false;
    }

    free(visible);

    return visible_count == tree->logical_size;
}
