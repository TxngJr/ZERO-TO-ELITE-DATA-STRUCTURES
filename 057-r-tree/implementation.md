# Implementation — IntRTree

Fixed teaching capacity:
- max entries M=4
- minimum non-root occupancy m=2

Node:
- leaf flag
- count
- rect[5]
- ids[5] for leaf entries
- child[5] for internal entries

Insertion:
1. choose subtree by least enlargement
2. insert recursively
3. update exact child MBR
4. propagate split if needed
5. split root by creating new root

Split:
- quadratic seed selection
- enlargement-difference distribution
