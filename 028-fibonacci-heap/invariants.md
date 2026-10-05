# Invariants — IntFibonacciHeap

1. size==0 iff min==NULL
2. every root parent==NULL
3. every root mark==false
4. every circular list has consistent left/right back-links
5. parent key <= child key
6. node.degree equals number of direct children
7. child.parent points to owner
8. every reachable node appears exactly once
9. reachable count==size
10. min points to a root with minimum root key

Marked non-root nodes encode child-loss history used by cascading cuts.
