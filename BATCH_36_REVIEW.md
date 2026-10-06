# Batch 36 — Negative Review and Audit

## Scope

- 106 Free List
- 107 Garbage-Collected Data Structures
- 108 Cache-Aware Data Structures

## Chapter 106 Review

Checked:
- managed storage allocation size is alignment-rounded with overflow guard
- allocation start uses uintptr_t alignment math
- first-fit request accounts for alignment prefix before deciding fit
- both-prefix-and-suffix split allocates required metadata before mutating free-list state
- release matches exact allocation start; interior/foreign/double frees are rejected
- free list remains address sorted
- adjacent free blocks are coalesced immediately
- validator checks free/free, alloc/alloc and alloc/free non-overlap
- randomized 50,000-operation workload ends as one free extent equal to full capacity

Audit finding fixed before publication:
- strict -Werror rejected compact pointer-offset code with -Wmisleading-indentation. It was rewritten explicitly rather than suppressing the warning.
- construction now checks capacity+alignment rounding overflow before aligned_alloc.

## Chapter 107 Review

Checked:
- collection uses a preallocated mark stack so marking does not allocate
- objects are marked before push, bounding mark-stack occupancy by capacity
- roots are explicit and counted
- outgoing public edges accept only null/current valid handles
- unreachable cycles are reclaimed
- sweep invalidates handles before slot reuse
- free-stack/live/root invariants are validated
- independent reference reachability is compared against every handle after collection

Audit finding fixed before publication:
- initial generation design would eventually wrap and theoretically allow an ancient stale handle to match a reused slot again. Handle generation was widened to uint64_t and slots are permanently retired at UINT64_MAX instead of wrapping.
- strict -Werror also caught compact collection/test control flow and it was rewritten.

## Chapter 108 Review

Checked:
- tile dimensions and all derived multiplications are overflow checked
- logical-to-physical mapping is constant time
- every tile has equal physical size
- edge padding is kept zero and excluded from logical copies/transposes
- dense differential test covers all 257×193 cells after 100,000 random writes
- row-order and tile-order sums are equal
- tiled transpose is compared cell-by-cell against dense reference
- benchmark documentation does not claim tiled layout universally wins

## Local Verification

Compiler:
```text
-std=c17 -Wall -Wextra -Wpedantic -Werror
```

Sanitizers:
- Chapter 106 tests: ASan/UBSan PASS
- Chapter 107 tests: ASan/UBSan PASS
- Chapter 108 tests: ASan/UBSan PASS

## Claims Kept Narrow

- FreeListAllocator uses system-heap metadata and linear searches; it is a teaching free-list allocator, not a production malloc replacement.
- GcHeap is exact, non-moving, stop-the-world teaching mark-sweep with explicit roots; it is not a conservative or concurrent collector.
- CacheAwareMatrix uses an explicit tile parameter; benchmark results are workload/machine dependent and not a proof of universal speedup.
- Chapters 106–108 are not thread-safe.

## Definition of Done

Batch 36 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 108 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
