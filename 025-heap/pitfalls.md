# Pitfalls — Heap

- thinking heap array is globally sorted
- claiming BUILD-HEAP Theta(n log n)
- wrong last internal node
- unsigned backward-loop underflow
- choosing wrong child during sift-down
- using min-heap directly for ascending in-place heapsort logic without adjusting algorithm
- assuming arbitrary search O(log n)
- child-index arithmetic overflow
- confusing heap structure with process heap memory
