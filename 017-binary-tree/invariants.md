# Invariants — IntBinaryTree

1. size==0 iff root==NULL
2. root->parent==NULL
3. every left/right child points back to its parent
4. left and right of one parent cannot be same non-null node
5. every reachable node belongs to exactly one path from root
6. no cycle
7. reachable-node count==size
8. Tree owns all reachable nodes

Mutation must preserve these invariants before returning success.
