# Batch 05 — Negative Review and Audit

## Scope

- 013 Hashing Fundamentals
- 014 Hash Table
- 015 Hash Set / Hash Map

## Beginner review

Risk: learner believes hash value uniquely identifies key.
Fix: equality-vs-hash contract and explicit collision examples.

Risk: learner memorizes O(1) without assumptions.
Fix: every chapter separates expected, amortized and worst-case claims.

Risk: learner deletes an open-address slot by marking EMPTY.
Fix: tombstone probe-continuity example and tests.

## Correctness review

Hashing:
- equal-key/equal-hash contract
- length-aware embedded-zero hashing
- bucket-range tests

Separate chaining:
- unique-key update semantics
- pointer-to-pointer removal
- cycle detection in validation
- every entry validates against current bucket after rehash
- overflow-safe 75% threshold
- resize allocation before representation mutation

Open addressing:
- EMPTY/OCCUPIED/TOMBSTONE states
- lookup crosses tombstones
- tombstones reused
- string keys copied and owned
- cached hash validated against owned key
- same-capacity tombstone cleanup
- overflow-safe 75% live-load threshold

## Testing review

- 30,000 randomized chaining operations
- 20,000 randomized Set operations
- 25,000 randomized string-map operations
- owned-key mutation test
- tombstone survival test
- validators run continuously through random sequences

## Performance review

Benchmarks distinguish:
- hash distribution from lookup correctness
- expected hash lookup from linear lookup
- hash membership from linear-array membership

No benchmark is presented as a universal constant-time proof.

## Definition of Done

Batch 05 is complete only if Fedora CI:
- configures
- builds every target with warnings + ASan/UBSan
- passes the entire CTest suite

CI failure overrides any completion label.
