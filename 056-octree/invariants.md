# Invariants — IntOctree

1. valid half-open root box
2. every point lies in its leaf box
3. leaf has no children
4. internal node stores no direct points
5. internal node has 8 children
6. children exactly partition parent box
7. subtree_size is exact
8. tree size equals root subtree_size
9. node_count equals reachable nodes
10. depth <= max_depth
