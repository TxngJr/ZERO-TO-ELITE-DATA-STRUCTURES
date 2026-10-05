# Invariants — IntBinomialHeap

Heap-level:
1. root degrees strictly increase
2. root parent pointers are NULL
3. reachable nodes equal size

For B_k:
4. root degree=k
5. exactly k children
6. child root degrees k-1,k-2,...,0
7. parent key <= child key
8. every child recursively satisfies its B_degree structure

Duplicates are allowed.
