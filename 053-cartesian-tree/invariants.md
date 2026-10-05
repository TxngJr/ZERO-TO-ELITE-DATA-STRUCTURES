# Invariants — IntCartesianTree

1. node identity equals original sequence index
2. exactly one root for nonempty tree
3. root parent is SIZE_MAX
4. left child index < parent index
5. right child index > parent index
6. parent value <= child value
7. equal-value tie policy preserves earlier index above later equal chain where applicable
8. every node is reachable exactly once
9. inorder traversal is exactly 0,1,...,n-1
