# Batch 04 — Negative Review and Audit

## Scope
- 010 Queue
- 011 Deque
- 012 Priority Queue

## Beginner review

Risk: learner implements array queue by shifting on every dequeue.
Fix: CircularQueue separates logical head from physical index and explains why shifting is Θ(n).

Risk: learner treats physical ring order as logical queue order.
Fix: diagrams and growth tests explicitly separate both views.

Risk: learner thinks Deque and Queue are interchangeable concepts.
Fix: Deque is shown as a wider ADT; narrow APIs are still recommended when intent is FIFO/LIFO.

Risk: learner assumes Priority Queue is FIFO with a priority field.
Fix: ADT examples show extraction by maximum priority and explicitly discuss tie semantics.

## Correctness review

CircularQueue:
- safe wrapped indexing helper
- logical-order copy during growth
- canonical empty head reset
- wrap/growth test across many operations

LinkedQueue:
- last dequeue clears tail
- endpoint/node-count validation

IntDeque:
- unsigned-safe previous index
- front/back wrap helpers
- logical-order growth
- randomized operations at both ends

Monotonic Queue:
- expiry by index
- monotonic candidate dominance
- duplicate-value tests
- aggregate Θ(n) reasoning

MaxPriorityQueue:
- complete-tree array representation
- parent/child heap invariant
- sift-up/down
- equal-priority order deliberately unspecified
- randomized reference scan validates maximum priority and exact item membership

## Complexity review

Queue:
- circular enqueue Θ(1) amortized
- linked enqueue/dequeue Θ(1) at ADT-operation level

Deque:
- both-end operations Θ(1) amortized for pushes
- sliding-window maximum Θ(n) aggregate

Priority Queue:
- heap peek Θ(1)
- push/pop O(log n), Θ(log n) worst
- reference unsorted pop-max Θ(n)

Benchmarks are labeled as workload experiments rather than universal performance laws.

## Memory-safety review

Checked:
- geometric-growth multiplication guards
- no array shifting required for ring queues
- no unsigned head decrement
- node unlink before free
- heap backing realloc commit
- empty-state operations
- child indexing bounded by heap shape

## Definition of Done

Batch 04 is complete only when Fedora GitHub Actions:
- configures
- builds all targets with warnings + ASan/UBSan
- passes the complete CTest suite

A CI failure overrides documentation claiming completion.
