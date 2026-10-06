# Batch 34 — Negative Review and Audit

## Scope

- 100 Atomic Data Structures / CAS
- 101 Concurrent Hash Map
- 102 Concurrent Queue / Ring Buffer

## Chapter 100 Review

Checked:
- atomic bitset uses fetch-or/fetch-and/fetch-xor rather than non-atomic load+store
- bit operations return previous bit from the RMW result
- final-word CAS rejects desired states that set padding bits
- tagged value packs value + version in one atomic uint64_t
- successful CAS increments version exactly once
- stale snapshot after A→B→A is rejected because version changed
- weak CAS retry loop reports contention retries
- version/value overflow is explicitly rejected
- runtime atomic_is_lock_free guards progress claims
- 8-thread bitset and 8×10,000 tagged increments pass ASan/UBSan and TSan

## Chapter 101 Review

Checked:
- normal lock order is table read lock -> bucket mutex
- resize obtains table write lock only after insertion releases normal locks
- resize rechecks load condition under the write lock
- old bucket array is destroyed only while write lock excludes readers
- new buckets/mutexes are fully created before old state is modified
- resize allocation failure leaves logical insertion valid and current table usable
- atomic size matches actual node count in quiescent validator
- 8-thread insert/update/remove workload triggers repeated resizes and exact membership checks

Audit finding fixed before publication:
- initial draft compressed multiple control-flow statements on one source line. Strict `-Werror` rejected it with `-Wmisleading-indentation`. Source and tests were rewritten with explicit control-flow formatting before final repository blobs were created.

## Chapter 102 Review

Checked:
- capacity is fixed power-of-two and mask=capacity-1
- each cell owns atomic sequence generation metadata
- producer claims enqueue position with CAS before writing a cell
- payload write precedes release publication of ready sequence
- consumer acquire-loads ready sequence before reading payload
- consumed slot advances sequence by capacity before reuse
- 4 producers + 4 consumers transfer 100,000 values exactly once
- quiescent enqueue-dequeue difference is checked against capacity
- counter-wrap boundary is rejected instead of relying on signed-overflow tricks
- runtime lock-free atomic support is verified

Claim audit:
- an earlier conceptual temptation would be to label any CAS-based MPMC ring “lock-free”. The final chapter deliberately does not make that claim: a producer can reserve the logical front and stall before publishing its slot, so atomics/non-blocking try APIs alone are not sufficient evidence for a formal lock-free progress proof.

## Local Verification Before Commit

Strict compilation:
```text
-std=c17 -Wall -Wextra -Wpedantic -Werror -pthread
```

Sanitizers:
- Chapter 100: plain, ASan/UBSan and TSan PASS
- Chapter 101: plain, ASan/UBSan and TSan PASS
- Chapter 102: plain, ASan/UBSan and TSan PASS

Benchmark smoke observed locally:
- tagged CAS: 800,000 increments with retry metrics
- concurrent hash map: 400,000 inserts, 14 resizes in local run
- MPMC ring: 1,000,000 transfers

## Definition of Done

Batch 34 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 102 passes.
2. Fedora TSan suite for Chapters 096–102 passes.
3. Source-of-truth state, roadmap, coverage, README, glossary and changelog are updated.
