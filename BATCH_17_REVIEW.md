# Batch 17 — Negative Review and Audit

## Scope

- 049 Interval Tree
- 050 Interval Heap
- 051 Range Tree

## Chapter 049 Review

Checked:
- all intervals use half-open [low,high)
- low < high is enforced
- exact duplicates are rejected while shared endpoints remain allowed
- BST order is lexicographic (low,high)
- every AVL refresh recomputes both height and max_high
- single/double rotations refresh augmentation in dependency order
- two-child deletion copies successor key then repairs metadata on return
- overlap predicate is low1 < high2 && low2 < high1
- touching endpoints are not overlap
- left pruning uses max_high > query.low
- all-overlap traversal also prunes by low ordering
- validator recomputes ordering, AVL balance, height, max_high and exact node count

## Chapter 050 Review

Checked:
- node representation stores low<=high
- odd-size final singleton is represented low==high
- element count and node count are distinct
- low endpoints obey min-heap containment
- high endpoints obey max-heap containment
- insertion into a singleton completes exactly one interval
- even-size insertion creates exactly one new singleton node
- min deletion takes replacement from the final structure and repairs low side
- max deletion performs symmetric high-side repair
- crossing an opposite endpoint during sift-down swaps the replacement through the interval boundary
- duplicate priorities remain valid
- randomized reference model is a duplicate-preserving unsorted bag
- validator checks parent interval containment directly rather than separately trusting two heaps

Complexity:
- peek min/max O(1)
- insert/pop min/pop max O(log n)
- geometric node-array growth gives standard amortized allocation cost

## Chapter 051 Review

Checked:
- source points are copied, so external mutation cannot silently change the index
- points are sorted by x/y/id for primary construction
- median recursion gives balanced static primary tree
- associated y arrays are merged from child arrays plus pivot rather than independently qsorted at each node
- y comparator has internal index tie-break so exact duplicate point records still have total order
- min_x/max_x are recomputed from subtree boundaries
- rectangle API is half-open on both axes
- full x-covered canonical subtree uses two y lower-bounds
- partial x nodes recurse while checking the pivot point once
- report first computes needed count, preventing partial output on insufficient capacity
- validator reconstructs the child+pivot y merge and checks exact subtree counts

Complexity:
- build Theta(n log n)
- count O(log^2 n)
- report O(log^2 n + k)
- storage Theta(n log n)

## Testing Review

- Interval Tree: 30,000 randomized insert/remove/query operations
- Interval Heap: 40,000 randomized duplicate-preserving DEPQ operations
- Range Tree: 30,000 randomized rectangle queries against naive scan
- Range Tree validates duplicate coordinates and primary sizes 1..129
- CI keeps a 60-second per-test watchdog to turn accidental infinite loops into named failures

## Definition of Done

Batch 17 is complete only when Fedora CI configures/builds Chapters 001–051 with warnings + ASan/UBSan and the full CTest suite passes.
