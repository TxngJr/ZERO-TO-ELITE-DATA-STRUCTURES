# Invariants — IntRangeTree

1. primary points are sorted by x order used during median construction
2. primary tree is height-balanced by static median splitting
3. min_x/max_x equal subtree coordinate extrema
4. node y_count equals subtree point count
5. y_indices contains exactly the subtree point indices
6. y_indices is sorted by y comparator
7. every point belongs to exactly one primary node
8. query full-cover y slices never include points outside requested y range
