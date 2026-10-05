# Batch 22 — Negative Review and Audit

## Scope

- 064 Bitset
- 065 Bitmap
- 066 Bit Vector

## Chapter 064 Review

Checked:
- logical index maps to word i/64 and offset i%64
- shifts are performed on uint64_t
- no shift-by-64 path in final-mask construction
- popcount only sees logical data because padding stays zero
- ctz is called only when the scanned word is nonzero
- set_all, flip_all and NOT restore final-word padding mask
- AND/OR/XOR require equal logical lengths
- zero-length set is valid and all() follows vacuous truth semantics
- randomized workload explicitly crosses 63/64/65

Complexity:
- bit mutation/access O(1)
- aggregate/bitwise operations O(W)
- storage Theta(W)

## Chapter 065 Review

Checked:
- universe is [0,U)
- add/remove update cached cardinality only on actual membership transition
- range APIs are half-open and allow empty ranges
- range masks avoid shifting by 64
- cardinality delta is computed from before/after word popcount
- final padding bits are masked
- union/intersection/difference require identical universes
- result cardinality is recomputed exactly
- next_member calls ctz only on nonzero words
- validator recomputes cardinality and checks padding

Complexity:
- membership O(1)
- cardinality O(1)
- range mutation O(words touched)
- set algebra O(W)

## Chapter 066 Review

Checked:
- input accepts only bytes 0 or 1
- packed final padding is zero
- prefix_ones length is W+1
- prefix directory recurrence equals per-word popcount
- rank semantics are [0,end)
- rank1/rank0 accept end==n
- select semantics are 0-based
- select1 binary search locates word whose cumulative ones cross k
- select0 derives zero prefixes from logical bits, not storage capacity
- final-word complement is masked before zero selection
- local select uses ctz only after ensuring nonzero word
- immutable API prevents stale prefix metadata
- full per-word directory is described as simple O(W) metadata, not falsely called succinct

Complexity:
- build O(n+W)
- access/rank O(1)
- select O(log W) with bounded <=64 local work
- storage O(W)

## Testing Review

- Bitset: boundary checks plus 50,000 randomized operations
- Bitmap: range/set algebra plus 40,000 randomized operations
- Bit Vector: exhaustive ranks/selects per random vector and sizes around every 64-bit boundary
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 22 is complete only when Fedora CI configures/builds Chapters 001–066 and the full CTest suite passes with sanitizers enabled.
