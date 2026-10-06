# Batch 33 — Negative Review and Audit

## Scope

- 097 Thread-Safe Queue / Map
- 098 Lock-Free Data Structures
- 099 Wait-Free Data Structures

## Chapter 097 Review

Checked:
- bounded ring queue keeps head/tail/size under one mutex
- condition waits use while predicates, not one-shot if checks
- close wakes both producers and consumers
- closed queue rejects new pushes but drains existing elements
- 4 producers + 4 consumers transfer 20,000 unique values exactly once
- striped map locks each bucket for get/put/remove
- map size has separate synchronization
- 8-thread insert/remove stress verifies exact membership
- destruction is documented as quiescent-only

Audit finding fixed before commit:
- first validator version locked all 64 map buckets simultaneously; local TSan deadlock detector hit its tracked-lock limit. Validator contract is quiescent, so it now locks one bucket at a time and validates total size afterward.

## Chapter 098 Review

Checked:
- head publication uses CAS with release semantics
- pop acquires published node state
- every push reserves a unique slot
- next_slot never advances past capacity
- popped nodes are not reused or reclaimed before destroy
- implementation therefore avoids ABA caused by node identity reuse
- runtime checks every atomic used in algorithm/instrumentation for platform lock-free support
- 8-thread push/pop and mixed 4-producer/4-consumer tests verify exactly-once values
- CAS failure counters expose contention

Audit finding fixed before commit:
- early size bookkeeping happened after head publication. A concurrent pop could remove the newly published node before its push incremented size, causing unsigned underflow. Size is now incremented before publication and explicitly documented as approximate during an in-flight push but exact after quiescence.
- slot reservation was changed from unbounded fetch_add to bounded CAS reservation so lifetime capacity cannot wrap and accidentally reuse a slot.

## Chapter 099 Review

Checked:
- exactly one producer writes tail
- exactly one consumer writes head
- data is written before release publication of tail
- consumer acquire-loads tail before reading payload
- consumed head is release-published before producer reuses wrapped storage
- each try_push/try_pop contains no retry loop
- full/empty returns complete in bounded local work
- one physical slot is reserved to distinguish full from empty
- 500,000 values arrive in exact FIFO order
- platform atomic lock-free assumption is tested at runtime
- caller-level full/empty retry counters are documented separately from operation wait-freedom

## Local Verification Before Commit

Strict build:
```text
-Wall -Wextra -Wpedantic -Werror
```

Sanitizers:
- ASan/UBSan: 6 / 6 Batch 33 demo+test CTests passed
- TSan: 6 / 6 Batch 33 demo+test CTests passed
- no ThreadSanitizer race reports

Benchmark smoke:
- thread-safe queue: 400,000 transfers
- lock-free stack: 800,000 concurrent pushes with CAS-failure metric
- wait-free SPSC ring: 2,000,000 transfers

## Claims Kept Narrow Deliberately

- Chapter 097 structures are blocking/thread-safe, not lock-free.
- Chapter 098 is lock-free only when the used atomics report lock-free; the teaching design is bounded by lifetime push capacity and defers reclamation to destroy.
- Chapter 099 wait-free claim applies to individual SPSC try operations under the single-producer/single-consumer + lock-free atomic assumptions, not to arbitrary caller retry loops or MPMC usage.

## Definition of Done

Batch 33 is complete only when:
1. Fedora ASan/UBSan full repository build/tests through Chapter 099 pass.
2. Fedora TSan tests for Chapters 096–099 pass.
3. Source-of-truth state, roadmap, coverage, README, glossary and changelog are updated.
