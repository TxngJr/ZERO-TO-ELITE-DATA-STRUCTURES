# Implementation

Implicit treap node stores left/right, priority, subtree total and one byte. Heap priority keeps expected balance; in-order traversal defines text order.
Split by left-count mutates pointers but allocates nothing. Insert constructs new nodes first, then splits/merges.
