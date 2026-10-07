# Batch 39 — Negative Review and Audit

## Scope

- 115 Operating-System Data Structures
- 116 Networking Data Structures
- 117 AI/ML Data Structures

## Chapter 115 Review

Checked:
- process slots are partitioned into active and free sets
- PIDs pack generation + slot and stale handles fail after reuse
- every READY process appears exactly once in its matching priority queue
- BLOCKED/RUNNING processes are absent from ready queues
- single-CPU model has at most one RUNNING process
- yield/block/wake correctly move state and queue membership
- terminating READY process removes it from its queue
- changing READY priority migrates queue membership
- validator cross-checks queue head/tail/count, active count and free stack

Audit finding fixed before publication:
- initial compact source draft failed strict `-Werror` with `-Wmisleading-indentation`; the final source was rewritten with explicit control flow and re-run under ASan/UBSan.

Claims kept narrow:
- strict-priority scheduling can starve lower priorities
- READY termination/priority migration scan a singly linked queue
- this is a single-threaded teaching scheduler, not a production SMP scheduler

## Chapter 116 Review

Checked:
- prefix bits are consumed MSB-first
- /0 default route is represented at root
- lookup retains deepest matching route
- trie children use stable node indices rather than raw pointers across realloc
- route remove clears route metadata without invalidating descendants
- flow key equality compares fields explicitly
- deletion uses tombstones rather than breaking probe chains with EMPTY
- rehash removes tombstones and retains every live flow
- flow validator re-resolves every USED key/value

Claims kept narrow:
- binary trie is not Patricia-compressed or IPv6-capable
- expected O(1) flow operations depend on hash/probe behavior
- route removal does not reclaim path nodes

## Chapter 117 Review

Checked:
- dataset sizes are overflow checked before allocation
- row access/gather reject uninitialized rows
- batch plan stores indices instead of physically shuffling feature rows
- Fisher-Yates produces a full permutation under deterministic seed
- validator checks every row index exactly once
- final batch may be shorter than requested batch size
- embedding rows are contiguous and initialized before gather
- 10,000 gathered embeddings match source rows exactly

Claims kept narrow:
- gather copies data; it is not a zero-copy tensor view
- the chapter does not implement GPU device memory, autograd or distributed data loaders
- nearest-neighbor/vector search is intentionally deferred to Chapter 118

## Local Verification

Compiler:
```text
-std=c17 -Wall -Wextra -Wpedantic -Werror
```

Sanitizers:
- Chapter 115 core tests: ASan/UBSan PASS
- Chapter 116 core tests: ASan/UBSan PASS
- Chapter 117 core tests: ASan/UBSan PASS

Benchmark smoke:
- scheduler: 500,000 dispatch/yield cycles
- networking: 200,000 LPM lookups + 100,000 flow insert/lookups
- AI/ML: 200,000-row dataset gather epoch + 200,000 embedding gathers

## Definition of Done

Batch 39 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 117 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
