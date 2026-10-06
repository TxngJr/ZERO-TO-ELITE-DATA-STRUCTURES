# Batch 31 — Negative Review and Audit

## Scope

- 091 Succinct Data Structures
- 092 Persistent Data Structures
- 093 Immutable Data Structures

## Chapter 091 Review

Checked:
- packed payload uses uint64_t words
- input bytes are validated as exactly 0/1
- sizes 0,1,2,63,64,65,511,512,513,1023,1024,1025 are covered
- rank contract is half-open [0,end)
- final-word padding is forced to zero and masked during select0
- super_rank checkpoints are recomputed by validator
- select1/select0 reject out-of-range occurrence indexes
- storage accounting includes payload, directory and object metadata
- documentation explicitly does not mislabel the fixed-512-bit directory as a proof of n+o(n) succinctness
- randomized 200,000-bit differential workload runs 5,000 rank, 5,000 select1 and 5,000 select0 queries against naive reference

Complexity:
- access O(1)
- rank O(1) under the fixed eight-word local scan
- select O(log superblocks) plus bounded local scan
- validation O(n/64)

## Chapter 092 Review

Checked:
- published nodes are immutable and child pointers are const
- arbitrary historical roots can be updated to branch history
- stable addresses come from fixed blocks, never reallocating node storage
- path copying shares untouched subtrees
- duplicate insert and missing erase allocate no nodes
- two-child erase copies the successor-removal path without mutating old nodes
- subtree-size metadata is validated recursively
- arena mark/rollback prevents a failed allocation from publishing a partial version
- old roots are repeatedly rechecked after later updates
- arena lifetime/reclamation limitation is documented instead of hidden
- plain BST worst-case Theta(n) is stated; no false O(log n) guarantee

Complexity:
- contains O(h)
- changed insert/erase O(h) time and O(h) new nodes
- no-op update O(h) time and O(1) new nodes
- arena memory grows with retained history

## Chapter 093 Review

Checked:
- constructor copies keys and coalesces duplicates
- capacity is power of two and at least twice worst-case input occupancy
- open addressing uses linear probing with no tombstones because mutation/deletion are absent
- signed integer hashing is deterministic
- INT_MIN/INT_MAX and duplicates are tested
- invariant checks occupied-count, load policy and probe reachability
- API exposes no insert/remove mutation
- docs distinguish immutable, persistent, copy-on-write and read-only concepts
- concurrency discussion does not claim lifetime/publication automatically safe
- 100,000-input randomized differential test compares all 50,000-domain memberships and 20,000 guaranteed misses

Complexity:
- expected build O(n), pathological probing can degrade
- expected contains O(1), worst O(capacity)
- storage accounting includes keys plus occupancy bytes

## Local Verification Before Commit

Compiled with:

```text
gcc -std=c17 -Wall -Wextra -Wpedantic -fsanitize=address,undefined
```

All three tests, demos and benchmarks completed locally under ASan/UBSan. The Batch 31 commit must still pass the repository Fedora GitHub Actions workflow before the batch is declared complete.

## Definition of Done

Batch 31 is complete only when the repository configures Chapters 001–093, builds all targets, runs the full CTest suite with sanitizers, and the GitHub Actions workflow concludes success.
