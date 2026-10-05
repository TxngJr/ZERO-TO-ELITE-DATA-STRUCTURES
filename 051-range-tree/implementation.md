# Implementation — IntRangeTree

Point:
- int64_t x
- int64_t y
- uint64_t id

Tree owns a sorted copy of all input points.

Node:
- point_index
- left/right
- min_x/max_x
- y_indices[]
- y_count

Build:
- sort points by x
- recursive median primary tree
- merge associated y arrays

Query:
- x pruning/full-cover detection
- y lower-bound searches on canonical subtrees
