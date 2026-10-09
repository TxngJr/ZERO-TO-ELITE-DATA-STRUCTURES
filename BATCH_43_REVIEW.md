# Batch 43 — Negative Review and Audit

## Scope

- 127 Locality of Reference
- 128 CPU Cache Effects
- 129 False Sharing

## Chapter 127 Review

Checked:
- locality correctness is based on trace geometry, never wall-clock timing
- index × element-size multiplication is overflow checked
- line 0 is represented with an explicit used bit rather than a sentinel key
- unique line count and temporal reuse satisfy accesses = unique + reuse
- every adjacent pair is classified as same-line or line-transition
- adjacent-line transitions are a subset of transitions
- sequential 128×8-byte trace over 64-byte lines gives exactly 16 lines, 112 same-line adjacent pairs and 15 transitions
- repeated sequential scans reuse the same 16 lines
- modular strided generator requires gcd(count,stride)=1 before claiming a permutation

Claims kept narrow:
- locality metrics do not equal hardware cache hit rate
- benchmark timing is observational and not a correctness assertion

## Chapter 128 Review

Checked:
- address → line → set/tag mapping is explicit
- each set stores a unique valid MRU→LRU prefix
- hit moves the matching tag to MRU without changing occupancy
- non-full miss does not count eviction
- full miss evicts the LRU tail exactly once
- timestamp-free LRU avoids replacement-clock overflow
- hits + misses == accesses
- direct-mapped conflict trace has exact expected counts
- 2-way LRU trace has exact expected counts
- 64×64 int row-major traversal yields exactly 256 line misses for the selected geometry
- column-major trace is required only to have more misses than row-major under that same geometry

Claims kept narrow:
- no prefetcher, write policy, hierarchy, coherence or cycle timing
- this is a deterministic teaching simulator, not a specific commercial CPU model

## Chapter 129 Review

Checked:
- logical counters are C atomics, not volatile or racy plain integers
- base storage is explicitly cache-line aligned while retaining raw allocation for free
- stride and line-size constraints preserve atomic alignment
- count × stride and alignment slack arithmetic are overflow checked
- packed stride places 8 uint64 counters in one modeled 64-byte line on the Fedora/x86-64 target
- 64-byte stride gives one counter per modeled line
- layout queries are deterministic and independent of scheduler timing
- worker threads update distinct atomic counters with memory_order_relaxed
- all successfully created threads are joined even if a later pthread_create fails
- test verifies exact final counts and reset semantics
- timing benchmark has no pass/fail performance threshold

CI scope change:
- Chapter 129 is inherently concurrent, so TSan is expanded from Chapters 096–102 to Chapters 096–102 plus 129.
- TSan builds/tests only the Chapter 129 demo and correctness tests, not its timing benchmark.

Claims kept narrow:
- modeled line sharing is relative to the aligned allocation and requested line size; it does not discover physical cache sets
- padding may reduce false sharing but increases memory footprint and is not guaranteed to improve every run

## CI Definition of Done

Batch 43 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 129 passes.
2. Expanded Fedora TSan suite for Chapters 096–102 and 129 passes.
3. README, state, coverage, roadmap, glossary, changelog and workflow scope are updated.
