# Batch 09 — Negative Review and Audit

## Scope

- 025 Heap
- 026 D-ary Heap
- 027 Binomial Heap

## Chapter 025 Review

Checked:
- chapter extends rather than repeats Chapter 012
- complete-tree shape separated from heap-order invariant
- BUILD-HEAP is stated Theta(n), not n log n
- unsigned backward loops avoid underflow
- child arithmetic is only formed after live-child bounds are established
- replace chooses sift-up for decreased values and sift-down for increased values
- Heap Sort uses a max-heap to produce ascending in-place order
- arbitrary key lookup is not falsely claimed logarithmic

## Chapter 026 Review

Checked:
- arity d<2 rejected
- parent formula (i-1)/d
- overflow-safe first-child precondition before d*i+1
- child scan limited to live contiguous children
- push O(log_d n)
- pop includes d-way scan: O(d log_d n)
- build remains linear via aggregate/geometric reasoning
- benchmarks are workload/machine dependent, not universal d rankings

## Chapter 027 Review

Checked:
- root degrees strictly increase after consolidation
- three-equal-degree case is handled before linking
- linked child is prepended, giving child degree order k-1...0
- extract-min reverses descending child list before root-list merge
- promoted roots get parent=NULL
- destructive meld empties source without copying nodes
- duplicate keys are allowed
- peek-min is O(log n) because min is not cached
- validator checks full B_k shape, heap order, parent links and total size

## Handle Semantics

Decrease-key swaps integer key payloads upward.
Therefore a node handle is a storage-node handle, not stable logical-item identity.

Delete-handle:
- first climbs to the tree root
- verifies that root is present in this heap
- only then bubbles the target key to root
- removes that root and melds its children

Dangling handles remain invalid by API precondition.

## Testing Review

- Heap random: 30,000 operations
- D-ary random: 20,000 operations per d for five arities
- Binomial random: 25,000 operations
- duplicate keys and foreign delete handles covered
- validators run continuously after mutations
- heapsort compared against qsort

## Complexity Contrast

Binary Heap:
- peek Theta(1)
- build Theta(n)
- strong locality

D-ary Heap:
- tuned height/child-scan trade-off

Binomial Heap:
- pointer forest
- meld O(log(n+m))
- peek O(log n) without cached min

## Definition of Done

Batch 09 is complete only when Fedora CI configures, builds with ASan/UBSan and passes the complete CTest suite for Chapters 001–027.
