# Batch 23 — Negative Review and Audit

## Scope

- 067 Bit Trie / XOR Trie
- 068 Bloom Filter
- 069 Counting Bloom Filter

## Chapter 067 Review

Checked:
- fixed 64-level path from bit 63 to bit 0
- uint64_t shifts avoid signed-bit ambiguity
- duplicates represented by terminal_count and subtree_count
- insert reserves capacity for worst-case 64 new nodes before mutation
- remove first verifies full exact path, then decrements all 65 path counts
- historical zero-count nodes remain reachable intentionally
- query traversal ignores zero-count child branches
- max XOR prefers opposite bit, min XOR same bit
- threshold query implements strict XOR < limit
- validator checks internal count=sum children, leaf count=terminal count, root count=size

Complexity:
- fixed uint64_t operations O(64)
- general w-bit operations O(w)
- no dead-node reclamation in this teaching implementation

## Chapter 068 Review

Checked:
- bit_count/hash_count must be positive
- binary keys and empty byte key are supported
- NULL key pointer allowed only for zero length
- double-hash positions are always reduced into bit_count
- insert only sets bits; it never clears
- final padding bits remain zero because no generated index reaches padding
- inserted keys are explicitly retested to establish no false negatives in tests
- negative probes are used only to observe/bound this deterministic test workload's false-positive count, not claim universal rate
- positive result is documented as maybe-present only
- duplicate insertion count is API-call count, not distinct-key cardinality

Complexity:
- add/query O(k)
- storage O(m bits)

## Chapter 069 Review

Checked:
- uint16 counters never wrap
- add rolls back every earlier increment if any probe hits UINT16_MAX
- rollback works even when one key maps multiple hashes to same counter
- remove rolls back earlier decrements if a later probe reaches zero/underflow condition
- nonzero_count is updated on 0<->1 transitions and validator recomputes it
- logical_count tracks successful add minus successful remove calls, not distinct keys
- delete semantics explicitly require caller knowledge that the occurrence is valid
- deleting an arbitrary false-positive key is documented as unsafe because it can steal contributions from other keys
- removed keys may still query positive because of collisions
- one-counter saturation test reaches UINT16_MAX and verifies failed extra add does not mutate logical metadata

Complexity:
- add/remove/query O(k)
- storage O(m counters)

## Testing Review

- XOR Trie: 50,000 randomized operations against explicit multiset model
- Bloom Filter: 5,000 inserted keys retested for no false negatives and 10,000 disjoint probes
- Counting Bloom: duplicates, saturation rollback, valid deletion and 20,000 randomized add/remove operations
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 23 is complete only when Fedora CI configures/builds Chapters 001–069 and the full CTest suite passes with sanitizers enabled.
