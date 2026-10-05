# Batch 28 — Negative Review and Audit

## Scope

- 082 Circular Buffer / Ring Buffer
- 083 Gap Buffer
- 084 Rope

## Chapter 082 Review

Checked:
- capacity must be positive
- explicit size distinguishes full from empty
- reject-on-full semantics are documented and tested
- logical index mapping wraps without moving data
- push derives tail from head + logical size
- pop advances head and normalizes empty state to head=0
- front/back/get reject empty/out-of-range states
- validator checks head/capacity/size and every live logical mapping
- 50,000 randomized operations compare against an explicit FIFO reference model

Complexity:
- push/pop/front/back/get O(1)
- clear O(1)
- storage Theta(capacity)

## Chapter 083 Review

Checked:
- gap_start <= gap_end <= capacity
- logical size excludes physical gap bytes
- move-left and move-right use memmove for overlapping regions
- growth preserves prefix and moves suffix to the new right edge
- insert validates position and byte pointer
- erase validates half-open range and enlarges the gap
- copy emits prefix followed by suffix only
- validator checks prefix/suffix arithmetic
- implementation is explicitly byte-oriented; UTF-8 code-point safety is not falsely implied
- allocation failure during growth may leave the gap cursor moved but leaves logical text unchanged

Complexity:
- get O(1)
- local insert amortized near O(length)
- gap movement O(distance)
- growth O(n)

## Chapter 084 Review

Checked:
- implicit treap in-order traversal defines logical byte sequence
- subtree total is recomputed after every split/merge pointer mutation
- parent priority is >= child priorities
- split allocates nothing
- insert builds the new temporary treap before splitting the existing root
- allocation failure frees temporary nodes and restores RNG state, leaving original sequence unchanged
- erase uses two splits, frees middle tree, merges survivors
- get navigates by left-subtree total
- validator uses overflow-safe left+1+right checks
- implementation clearly labels one-byte leaves as pedagogical and memory-heavy
- randomized treap balance is expected/probabilistic, not deterministic worst-case O(log n)

Complexity:
- get expected O(log n)
- split/merge expected O(log n)
- insert includes per-byte node construction
- copy O(n)
- erase includes deleted-node reclamation

## Testing Review

- Ring Buffer: 50,000 randomized operations with continuous structural validation
- Gap Buffer: 20,000 randomized edits with full byte-for-byte comparison after each operation
- Rope: 20,000 randomized edits with full byte-for-byte comparison and structural validation after each operation
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 28 is complete only when Fedora CI configures/builds Chapters 001–084 and the full CTest suite passes with sanitizers enabled.
