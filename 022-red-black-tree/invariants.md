# Invariants — IntRedBlackTree

BST:
1. keys unique
2. all left keys < node key
3. all right keys > node key

Structure:
4. root parent NULL
5. child parent links agree
6. reachable count=size

Red-Black:
7. root BLACK
8. NULL children logically BLACK
9. RED node has BLACK children
10. left/right subtree black-height equal at every node

If all hold, height has logarithmic bound.
