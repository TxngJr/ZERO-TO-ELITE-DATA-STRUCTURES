# Implementation — IntIntervalTree

Node fields:
- low
- high
- max_high
- height
- left
- right

Ordering:
    (low,high) lexicographic

Balancing:
    AVL rotations

Public API:
- create/free
- size
- insert/remove/contains
- find_overlap
- count_overlaps
- validate
