# Invariants — IntTreap

BST:
1. keys unique
2. left keys < node key
3. right keys > node key

Heap:
4. parent pair (priority,key) <= each child pair

Structure:
5. root parent NULL
6. child parent links agree
7. reachable count=size

Randomness is not an invariant.
It is an assumption used for expected-complexity analysis.
