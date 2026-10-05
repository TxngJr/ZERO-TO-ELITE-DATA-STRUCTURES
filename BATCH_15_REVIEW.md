# Batch 15 — Negative Review and Audit

## Scope

- 043 Segment Tree
- 044 Lazy Propagation Segment Tree
- 045 Dynamic Segment Tree

## Chapter 043 Review

Checked:
- every public interval uses half-open [left,right)
- recursive midpoint cannot overflow size_t because it uses left+(right-left)/2
- build is Theta(n), not repeated O(log n) updates
- leaf stores sum=min=value
- internal sum/min are exact child aggregates
- point set updates exactly one root-to-leaf path then pulls ancestors
- range query uses boolean partial-result validity rather than a fake minimum sentinel
- randomized test compares both sum and min against a naive array

Complexity:
- build Theta(n)
- point set O(log n)
- interval sum/min O(log n)
- storage Theta(n)

## Chapter 044 Review

Checked:
- full-cover add updates node sum/min and composes lazy tag
- sum update multiplies delta by interval length
- partial update pushes before descending
- pull occurs only after parent lazy has been cleared
- const query does not mutate tree; it passes ancestor deferred delta as carry
- validator checks:
  sum = left.sum + right.sum + lazy*length
  min = min(left.min,right.min) + lazy
- create rejects count larger than INT64_MAX because interval length is converted to int64_t
- randomized test compares range add/sum/min and point values against a naive array
- benchmark draft placeholder was caught and removed before commit

Complexity:
- range add O(log n)
- range sum/min O(log n)
- storage Theta(n)

Integer contract:
- callers must keep all stored int64 aggregates and tag arithmetic representable

## Chapter 045 Review

Checked:
- coordinate domain is uint64_t half-open [low,high)
- domain requires low < high
- midpoint uses low+(high-low)/2 to avoid addition overflow
- NULL child means an all-zero interval
- point add allocates only one coordinate path
- node sums are updated only after deeper recursion succeeds
- newly allocated empty chain is rolled back on allocation failure
- node_count overflow is guarded before allocation
- pruning only removes sum-zero nodes with no children
- internal zero-sum nodes with cancelling descendants are preserved
- validator recomputes sums and exact reachable node count
- huge-domain test includes coordinates near both ends of [0,10^18)

Complexity:
- point add/get O(log U)
- range sum O(log U) canonical decomposition
- worst sparse storage O(K log U)

## Testing Review

- Chapter 043 randomized differential: 30,000 operations
- Chapter 044 randomized differential: 15,000 operations
- Chapter 045 randomized differential: 20,000 operations
- validators run periodically during mutation workloads
- all benchmark executables are build targets but are not CTest regression tests

## Definition of Done

Batch 15 is complete only when Fedora CI configures and builds Chapters 001–045 with warnings + ASan/UBSan and passes the full CTest suite.
