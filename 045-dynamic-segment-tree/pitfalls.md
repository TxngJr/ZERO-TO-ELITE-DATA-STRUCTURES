# Pitfalls — Dynamic Segment Tree

- using (left+right)/2 and overflowing coordinates
- allocating both children eagerly and losing sparsity
- treating NULL as unknown instead of zero
- pruning internal zero-sum node whose children contain cancelling values
- leaking partially allocated path after malloc failure
- confusing O(log U) with O(log K)
- using a signed coordinate difference that overflows
