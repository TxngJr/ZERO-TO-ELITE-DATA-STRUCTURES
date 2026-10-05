# Invariants — IntSplayTree

Structural/BST:
1. keys unique
2. left keys < node key
3. right keys > node key
4. root parent NULL
5. child parent links agree
6. reachable count=size

Splay transformation preserves all six.

There is deliberately no AVL balance factor, stored height or Red-Black color invariant.
