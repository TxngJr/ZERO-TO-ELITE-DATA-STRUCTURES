# Implementation — IntKDTree

Node:
- point (x,y,id)
- split axis
- left/right node indices
- subtree_size
- min/max x/y bounding box

Tree:
- contiguous node array
- root index
- count

Build:
- temporary point entries
- qsort current recursive subrange by split axis
- choose median
- alternate axis

This intentionally yields O(n log^2 n) build complexity.
