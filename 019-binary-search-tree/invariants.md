# Invariants — IntBST

1. size==0 iff root==NULL
2. root->parent==NULL
3. child->parent links agree
4. no cycles/shared child nodes
5. every key unique
6. for every node:
       all left keys < node.key
       all right keys > node.key
7. reachable count==size
8. inorder sequence strictly increasing
