# Complexity — Chapter 002

Formal asymptotic notation is deferred to Chapter 004.

Memory operations have more than "number of source lines" cost.

Important dimensions introduced here:
- number of allocations
- bytes allocated
- number of memory accesses
- locality of accesses
- pointer dependencies
- cache/TLB behavior

Two traversals can each visit n elements yet have different real latency because the access pattern differs.

Later chapters combine asymptotic analysis with memory hierarchy effects rather than replacing one with the other.
