# Theory — R-Tree

## Bounding hierarchy

Internal entries do not represent objects.

They summarize child subtrees with minimum bounding rectangles.

A search descends every child whose MBR can still contain an overlapping answer.

## Why overlap matters

If sibling MBRs overlap heavily, one query may visit many branches.

Insertion heuristics therefore try to minimize:
- area enlargement
- dead space
- overlap

This chapter implements least enlargement and quadratic split.

## Balanced leaves

Splits propagate upward.

Root growth is the only operation that increases tree height.

Therefore all object entries remain on one common leaf level.
