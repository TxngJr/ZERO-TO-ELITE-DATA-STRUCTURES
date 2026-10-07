#include "ml_structures.h"
#include <stdlib.h>
#include <string.h>

struct MlDataset {
    size_t rows;
    size_t features;
    float *x;
    int *labels;
    unsigned char *initialized;
};

struct BatchPlan {
    size_t rows;
    size_t batch_size;
    size_t cursor;
    size_t *order;
};

struct EmbeddingTable {
    size_t rows;
    size_t dims;
    float *values;
    unsigned char *initialized;
};

static bool mul_size(size_t a, size_t b, size_t *out) {
    if (a != 0U && b > SIZE_MAX / a) return false;
    *out = a * b;
    return true;
}

MlDataset *ml_dataset_create(size_t rows, size_t features) {
    if (rows == 0U || features == 0U) return NULL;
    size_t elements = 0U;
    if (!mul_size(rows, features, &elements) ||
        elements > SIZE_MAX / sizeof(float) ||
        rows > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    MlDataset *dataset = calloc(1, sizeof(*dataset));
    if (!dataset) return NULL;

    dataset->x = malloc(elements * sizeof(*dataset->x));
    dataset->labels = malloc(rows * sizeof(*dataset->labels));
    dataset->initialized = calloc(rows, 1U);
    if (!dataset->x || !dataset->labels || !dataset->initialized) {
        ml_dataset_free(dataset);
        return NULL;
    }

    dataset->rows = rows;
    dataset->features = features;
    return dataset;
}

void ml_dataset_free(MlDataset *dataset) {
    if (!dataset) return;
    free(dataset->x);
    free(dataset->labels);
    free(dataset->initialized);
    free(dataset);
}

bool ml_dataset_set_row(MlDataset *dataset, size_t row,
                        const float *features, int label) {
    if (!dataset || !features || row >= dataset->rows) return false;
    memcpy(dataset->x + row * dataset->features, features,
           dataset->features * sizeof(float));
    dataset->labels[row] = label;
    dataset->initialized[row] = 1U;
    return true;
}

const float *ml_dataset_row(const MlDataset *dataset, size_t row) {
    if (!dataset || row >= dataset->rows || !dataset->initialized[row])
        return NULL;
    return dataset->x + row * dataset->features;
}

bool ml_dataset_label(const MlDataset *dataset, size_t row, int *out_label) {
    if (!dataset || !out_label || row >= dataset->rows ||
        !dataset->initialized[row]) {
        return false;
    }
    *out_label = dataset->labels[row];
    return true;
}

size_t ml_dataset_rows(const MlDataset *dataset) { return dataset ? dataset->rows : 0U; }
size_t ml_dataset_features(const MlDataset *dataset) { return dataset ? dataset->features : 0U; }

bool ml_dataset_gather(const MlDataset *dataset, const size_t *indices,
                       size_t count, float *out_features, int *out_labels) {
    if (!dataset || (count && (!indices || !out_features || !out_labels)))
        return false;

    for (size_t i = 0; i < count; ++i) {
        size_t row = indices[i];
        if (row >= dataset->rows || !dataset->initialized[row]) return false;
        memcpy(out_features + i * dataset->features,
               dataset->x + row * dataset->features,
               dataset->features * sizeof(float));
        out_labels[i] = dataset->labels[row];
    }
    return true;
}

bool ml_dataset_validate(const MlDataset *dataset) {
    return dataset && dataset->rows > 0U && dataset->features > 0U &&
           dataset->x && dataset->labels && dataset->initialized;
}

static uint64_t splitmix64(uint64_t *state) {
    uint64_t z = (*state += UINT64_C(0x9e3779b97f4a7c15));
    z = (z ^ (z >> 30U)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27U)) * UINT64_C(0x94d049bb133111eb);
    return z ^ (z >> 31U);
}

static void shuffle_order(size_t *order, size_t rows, uint64_t seed) {
    uint64_t state = seed;
    for (size_t i = rows; i > 1U; --i) {
        size_t j = (size_t)(splitmix64(&state) % i);
        size_t tmp = order[i - 1U];
        order[i - 1U] = order[j];
        order[j] = tmp;
    }
}

BatchPlan *batch_plan_create(size_t rows, size_t batch_size, uint64_t seed) {
    if (rows == 0U || batch_size == 0U || rows > SIZE_MAX / sizeof(size_t))
        return NULL;

    BatchPlan *plan = calloc(1, sizeof(*plan));
    if (!plan) return NULL;
    plan->order = malloc(rows * sizeof(*plan->order));
    if (!plan->order) { free(plan); return NULL; }

    plan->rows = rows;
    plan->batch_size = batch_size;
    for (size_t i = 0; i < rows; ++i) plan->order[i] = i;
    shuffle_order(plan->order, rows, seed);
    return plan;
}

void batch_plan_free(BatchPlan *plan) {
    if (!plan) return;
    free(plan->order);
    free(plan);
}

bool batch_plan_next(BatchPlan *plan,const size_t **out_indices,size_t *out_count,bool *out_has_batch) {
    if (!plan || !out_indices || !out_count || !out_has_batch) return false;
    if (plan->cursor >= plan->rows) {
        *out_indices = NULL; *out_count = 0U; *out_has_batch = false; return true;
    }

    size_t remaining = plan->rows - plan->cursor;
    size_t count = remaining < plan->batch_size ? remaining : plan->batch_size;
    *out_indices = plan->order + plan->cursor;
    *out_count = count;
    *out_has_batch = true;
    plan->cursor += count;
    return true;
}

bool batch_plan_reset(BatchPlan *plan,uint64_t seed,bool reshuffle) {
    if (!plan) return false;
    plan->cursor = 0U;
    if (reshuffle) shuffle_order(plan->order, plan->rows, seed);
    return true;
}

size_t batch_plan_batch_size(const BatchPlan *plan) { return plan ? plan->batch_size : 0U; }

bool batch_plan_validate(const BatchPlan *plan) {
    if (!plan || !plan->order || plan->rows == 0U ||
        plan->batch_size == 0U || plan->cursor > plan->rows) return false;

    unsigned char *seen = calloc(plan->rows, 1U);
    if (!seen) return false;
    bool ok = true;
    for (size_t i = 0; i < plan->rows; ++i) {
        size_t index = plan->order[i];
        if (index >= plan->rows || seen[index]) { ok = false; break; }
        seen[index] = 1U;
    }
    free(seen);
    return ok;
}

EmbeddingTable *embedding_create(size_t rows,size_t dims) {
    if (rows == 0U || dims == 0U) return NULL;
    size_t elements = 0U;
    if (!mul_size(rows,dims,&elements) || elements > SIZE_MAX / sizeof(float)) return NULL;

    EmbeddingTable *table = calloc(1,sizeof(*table));
    if (!table) return NULL;
    table->values = malloc(elements * sizeof(*table->values));
    table->initialized = calloc(rows,1U);
    if (!table->values || !table->initialized) { embedding_free(table); return NULL; }
    table->rows = rows; table->dims = dims; return table;
}

void embedding_free(EmbeddingTable *table) {
    if (!table) return;
    free(table->values); free(table->initialized); free(table);
}

bool embedding_set_row(EmbeddingTable *table,size_t row,const float *values) {
    if (!table || !values || row >= table->rows) return false;
    memcpy(table->values + row * table->dims, values, table->dims * sizeof(float));
    table->initialized[row] = 1U; return true;
}

const float *embedding_row(const EmbeddingTable *table,size_t row) {
    if (!table || row >= table->rows || !table->initialized[row]) return NULL;
    return table->values + row * table->dims;
}

bool embedding_gather(const EmbeddingTable *table,const size_t *indices,size_t count,float *out_values) {
    if (!table || (count && (!indices || !out_values))) return false;
    for (size_t i = 0; i < count; ++i) {
        size_t row = indices[i];
        if (row >= table->rows || !table->initialized[row]) return false;
        memcpy(out_values + i * table->dims,
               table->values + row * table->dims,
               table->dims * sizeof(float));
    }
    return true;
}

size_t embedding_rows(const EmbeddingTable *table) { return table ? table->rows : 0U; }
size_t embedding_dims(const EmbeddingTable *table) { return table ? table->dims : 0U; }
bool embedding_validate(const EmbeddingTable *table) {
    return table && table->rows > 0U && table->dims > 0U &&
           table->values && table->initialized;
}
