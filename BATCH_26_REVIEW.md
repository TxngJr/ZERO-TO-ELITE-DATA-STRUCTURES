# Batch 26 — Negative Review and Audit

## Scope

- 076 Count-Min Sketch
- 077 Count Sketch
- 078 Reservoir Sampling Structures

## Chapter 076 Review

Checked:
- width/depth must be positive
- each row uses a deterministic independent seed value
- exactly one counter per row receives each nonnegative update delta
- all target counters and total_weight are prechecked before mutation
- zero delta is a no-op
- estimate is the minimum row counter
- nonnegative collision mass means estimates cannot fall below exact frequency
- merge prechecks every counter and total weight before mutation
- validator recomputes each row sum and requires equality with total_weight
- documentation does not claim the deterministic mixer is a formal universal-hash proof

Complexity:
- add/query O(depth)
- merge O(width*depth)
- storage O(width*depth)

## Chapter 077 Review

Checked:
- width positive and depth positive odd
- independent deterministic seed arrays for bucket and sign
- signed updates allow positive/negative deltas
- INT64_MIN delta is rejected because unary negation is not representable
- every row update is overflow-prechecked before any counter mutation
- query reapplies row sign and rejects nonrepresentable negation of INT64_MIN counter
- median estimator is used instead of Count-Min's minimum
- merge prechecks all signed counter additions before mutation
- documentation explicitly states collision noise can move estimates in either direction
- teaching median uses insertion sort, so query includes O(depth^2) median work for small depth

## Chapter 078 Review

Checked:
- reservoir capacity must be positive
- sample_count always equals min(seen,capacity)
- first k items fill slots directly
- subsequent item t uses a uniform draw over [0,t)
- replacement occurs only when j < k
- bounded random integer uses rejection threshold before modulo, avoiding simple modulo bias
- zero user seed is mapped to a nonzero RNG state
- seen==UINT64_MAX rejects the next update before mutation
- deterministic same-seed streams yield identical reservoirs
- statistical test uses 30,000 independent seeded trials at k=1,N=10 with a broad +/-20% acceptance band; this checks implementation regressions rather than claiming a formal RNG-quality proof

Complexity:
- update expected O(1)
- get O(1)
- storage O(k)

## Testing Review

- CMS: 50,000 weighted nonnegative updates with exact model and no-underestimate checks
- Count Sketch: 50,000 signed turnstile updates with exact model and bounded deterministic absolute error
- Reservoir: invariants, deterministic reproducibility and repeated uniformity experiment
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 26 is complete only when Fedora CI configures/builds Chapters 001–078 and the full CTest suite passes with sanitizers enabled.
