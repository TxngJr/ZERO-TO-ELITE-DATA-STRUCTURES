#ifndef LOCALITY_TRACE_H
#define LOCALITY_TRACE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct {
    size_t accesses;
    size_t unique_lines;
    size_t first_line_touches;
    size_t temporal_line_reuses;
    size_t same_line_adjacent;
    size_t line_transitions;
    size_t adjacent_line_transitions;
    size_t max_index;
} LocalityStats;
bool loc_analyze_indices(const size_t *indices,size_t count,size_t element_size,size_t line_size,LocalityStats *out);
bool loc_generate_sequential(size_t count,size_t *out);
bool loc_generate_strided_permutation(size_t count,size_t stride,size_t *out);
#endif
