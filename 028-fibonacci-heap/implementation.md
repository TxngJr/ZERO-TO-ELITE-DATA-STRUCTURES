# Implementation — IntFibonacciHeap

Node:
- int key
- size_t degree
- bool mark
- parent/child
- left/right circular links

Heap:
- min
- size

Core helpers:
- list insert/remove
- concatenate root lists
- add_root
- link_under
- consolidate
- cut
- cascading_cut
- membership check
- extract_min_node
- recursive validation

Meld is destructive:
source becomes empty after successful concatenation.
