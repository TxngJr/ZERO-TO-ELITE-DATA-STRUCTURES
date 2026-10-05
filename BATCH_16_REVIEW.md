# Batch 16 — Negative Review and Audit

## Scope

- 046 Persistent Segment Tree
- 047 Fenwick Tree / Binary Indexed Tree
- 048 Sparse Table

## Chapter 046 Review

Checked:
- full persistence: new version may branch from any historical version
- published nodes are treated as immutable
- point-set clones one root-to-leaf path only
- untouched sibling subtrees are shared
- arena uses indices so realloc cannot invalidate child references
- update copies old node metadata before recursive allocations that may realloc
- version root is published only after complete successful update
- top-level node_count rollback discards unpublished path copies on failure
- node-count guard reserves index 0 safely and rejects near-SIZE_MAX arena overflow
- initial count guard prevents impossible 2n-style node-index overflow
- old historical snapshots are rechecked after all later updates

Complexity:
- initial build Theta(n)
- point-set version O(log n) time and O(log n) new nodes
- historical range query O(log n)
- m updates use O(n + m log n) nodes

## Chapter 047 Review

Checked:
- public indices/ranges are 0-based and half-open
- internal Fenwick indices are 1-based
- lowbit is implemented in unsigned size_t arithmetic
- update never calls lowbit(0)
- parent increment checks boundary before i += lowbit(i)
- prefix query repeatedly removes one binary block
- linear build propagates already-formed block sums to parent
- range sum uses prefix(right)-prefix(left)
- point set derives delta from point get
- integer contract explicitly requires representable set delta and prefix/block sums
- randomized test covers add, set, prefix and arbitrary range sum

Complexity:
- build Theta(n)
- point/prefix/range operations O(log n)
- storage Theta(n)

## Chapter 048 Review

Checked:
- immutable/static contract is explicit
- logs[len] stores floor(log2(len))
- level k represents intervals of length 2^k
- flattened levels*count allocation has multiplication overflow guards
- build only writes intervals fully inside [0,n)
- query uses two valid power-of-two blocks
- two blocks may overlap
- O(1) query is justified by min idempotence, not merely associativity
- sum is explicitly given as a counterexample to the overlap trick
- tests include non-power-of-two sizes 1..129

Complexity:
- build Theta(n log n)
- RMQ Theta(1)
- storage Theta(n log n)
- updates unsupported

## Testing Review

- Persistent Tree randomized branching workload: 6,000 steps plus 1,000 historical re-checks
- Fenwick randomized differential: 30,000 operations
- Sparse Table randomized differential: 50,000 queries
- structural validators cover each representation's defining recurrence

## Definition of Done

Batch 16 is complete only when Fedora CI configures/builds Chapters 001–048 with warnings + ASan/UBSan and the full CTest suite passes.
