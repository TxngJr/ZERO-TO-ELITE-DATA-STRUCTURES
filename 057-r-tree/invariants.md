# Invariants — IntRTree

1. every rectangle has xl<xh and yl<yh
2. leaf entries contain object rectangles/IDs
3. internal entries contain child pointers
4. internal entry rectangle equals exact MBR(child)
5. non-root node occupancy is [2,4]
6. root leaf may contain [0,4] entries
7. internal root contains [2,4] entries
8. all leaves occur at the same depth
9. tree.size equals total leaf object entries
10. tree.node_count equals reachable nodes
