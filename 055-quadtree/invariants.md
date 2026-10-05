# Invariants — IntQuadtree

1. root bounds satisfy low < high on both axes
2. every stored point lies inside its node bounds
3. leaf has no children
4. internal node stores no direct points
5. internal node has four child nodes
6. child rectangles exactly partition parent
7. subtree_size is exact
8. tree.size equals root subtree_size
9. tree.node_count equals reachable nodes
10. depth never exceeds max_depth
