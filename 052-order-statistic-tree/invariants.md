# Invariants — IntOrderStatTree

1. strict BST ordering
2. no duplicate keys
3. AVL balance factor in [-1,1]
4. cached height is exact
5. subtree_size = left_size+right_size+1
6. tree.size equals root subtree_size
7. rotations preserve inorder order
8. rank/select use subtree_size only after metadata repair
