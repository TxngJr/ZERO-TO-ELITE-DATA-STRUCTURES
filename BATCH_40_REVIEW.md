# Batch 40 — Negative Review and Audit

## Scope

- 118 Vector Search Structures
- 119 Game Data Structures
- 120 Merkle Tree

## Chapter 118 Review

Checked:
- vectors are stored contiguously row-major
- vector IDs equal append positions and remain stable across capacity growth
- non-finite and zero-norm vectors are rejected
- capacity growth allocates replacement vector/norm arrays before publishing them
- L2 ranking uses squared distance, avoiding unnecessary sqrt
- cosine search uses precomputed vector norms and validated nonzero query norm
- bounded heap root is the worst retained candidate
- equal-distance results use smaller ID as deterministic winner
- final results are sorted best-to-worst

Static findings fixed before commit:
- `FLT_MAX` usage required explicit `<float.h>`
- test use of `SIZE_MAX` required explicit integer-limit header
- math functions are linked through libm in CMake

Claims kept narrow:
- this is exact flat search, not ANN
- query complexity remains linear in vector count

## Chapter 119 Review

Checked:
- entity handles combine generation + slot
- destroy/reuse invalidates stale handles
- sparse-set swap-remove repairs moved entity's sparse index
- destroying an entity removes its Position component
- world version changes on identity/position mutation
- grid rebuild captures authoritative world version
- stale grid queries are rejected rather than silently returning outdated results
- grid query filters exact coordinates after scanning overlapping cells
- validator requires every positioned entity to appear in exactly one correct cell

Static findings fixed before commit:
- ambiguous one-line cell-clamping statements were split into explicit statements
- grid cell-count multiplication is overflow checked
- floating-point math dependencies are linked through libm

Claims kept narrow:
- grid is point-based and fixed-bound
- this is a single-threaded teaching ECS/spatial index

## Chapter 120 Review

Checked:
- SHA-256 known-vector test is independent from Merkle construction
- leaves use 0x00 domain prefix
- internal nodes use 0x01 domain prefix
- every level uses contiguous 32-byte hashes
- odd child counts duplicate the final hash consistently in builder/proof
- proofs copy siblings and therefore outlive the source tree
- verifier respects left/right sibling orientation
- full validator recomputes every internal node

Static finding fixed before commit:
- parent-count calculation originally used `(count + 1) / 2`, which can overflow at SIZE_MAX; final code uses `count/2 + count%2`.

Claims kept narrow:
- Merkle trees provide authenticated membership relative to a trusted root, not consensus or data-origin trust by themselves
- this chapter implements binary SHA-256 Merkle trees; Patricia structure is deferred to 121

## CI Definition of Done

Batch 40 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 120 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
