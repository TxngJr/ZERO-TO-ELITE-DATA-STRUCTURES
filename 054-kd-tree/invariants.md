# Invariants — IntKDTree

1. nonempty tree has exactly one reachable root
2. axis = depth mod 2
3. x-split left subtree max_x <= node.x
4. x-split right subtree min_x >= node.x
5. y-split left subtree max_y <= node.y
6. y-split right subtree min_y >= node.y
7. subtree_size is exact
8. cached bounding box exactly covers node + child boxes
9. every input record becomes exactly one node
10. median construction keeps child counts differing by at most one at each build split
