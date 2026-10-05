# Pitfalls — Interval Heap

- repairing only the min heap but breaking low<=high
- forgetting singleton semantics
- taking replacement from the wrong endpoint of the final full node
- choosing wrong child during sift-down
- assuming duplicates are invalid
- confusing element count with node count
- shrinking final node incorrectly after a pop
- implementing two unrelated heaps and calling it an interval heap
