# Pitfalls — Binomial Heap

- not reversing children after extract-min
- promoted root parent still non-NULL
- linking roots with different degree
- mishandling three equal-degree consecutive roots
- root list not degree-sorted
- assuming head root is minimum
- claiming peek O(1) without cached min
- treating key-swapping handle as stable item identity
- source heap not cleared after destructive meld
- validating heap order but not B_k shape
