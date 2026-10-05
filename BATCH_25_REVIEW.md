# Batch 25 — Negative Review and Audit

## Scope

- 073 Consistent Hashing
- 074 Probabilistic Data Structures
- 075 HyperLogLog

## Chapter 073 Review

Checked:
- virtual-node count must be positive
- physical node IDs are unique
- point_count equals node_count * virtual_nodes with overflow guard
- every physical node owns exactly the configured number of ring points
- points remain sorted by token/node/replica
- lookup lower-bounds the key token and wraps at ring end
- node removal compacts only that node's virtual points
- adding one node is tested over 50,000 keys: every ownership change goes to the newly added node in the deterministic test workload
- removing that node restores the exact prior ring and key ownership
- documentation does not claim perfect balance or zero movement
- qsort-based add complexity is stated honestly

## Chapter 074 Review

Checked:
- representative implementation is KMV rather than pretending the survey itself is a single universal structure
- k must be >=2
- retained hashes are unique
- duplicate input values do not consume repeated slots
- below k retained values the cardinality is exact for this deterministic bijective-style mixed input workload
- once full, only hashes below the current retained maximum enter the sample
- estimator uses (k-1)*2^64/R_k order-statistic form
- merge inserts retained hashes directly and does not hash them again
- equal k is required for merge
- implementation complexity is honestly O(k) per update due unsorted teaching array
- accuracy tests use relative-error bounds rather than exact-output claims

## Chapter 075 Review

Checked:
- precision restricted to p=4..18
- register count exactly 2^p
- high p bits choose register
- remaining hash bits determine leading-zero rank
- all-zero remainder gets maximum rank 65-p
- register update is monotonic max
- duplicate keys cannot decrease/change an already-equal register
- raw estimator uses alpha_m and harmonic register contribution
- zero-register small-range correction is applied
- 64-bit large-range correction is guarded by estimate range
- merge requires equal precision and uses component-wise maximum
- empty sketch estimates exactly zero through linear counting
- deterministic pre-CI check showed ~1.8% error for n=100,p=12 and ~0.64% for n=100000,p=14, within declared tests
- one-byte-per-register implementation is explicitly not presented as maximally compact

Complexity:
- HLL add O(1)
- estimate/merge O(m)
- storage O(m)

## Testing Review

- Consistent Hashing: 50,000 key assignments before/add/remove membership transition
- KMV: duplicate exact mode plus 50,000 cardinality estimate and merge equivalence
- HyperLogLog: empty, duplicate-heavy small set, 100,000 distinct accuracy and exact merged-register estimator equivalence
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 25 is complete only when Fedora CI configures/builds Chapters 001–075 and the full CTest suite passes with sanitizers enabled.
