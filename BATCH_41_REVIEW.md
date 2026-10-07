# Batch 41 — Negative Review and Audit

## Scope

- 121 Merkle Patricia Trie
- 122 Blockchain Data Structures
- 123 Graph Database Structures

## Chapter 121 Review

Checked:
- 32-byte keys are converted to exactly 64 nibbles
- Patricia nodes are Leaf / Extension / Branch
- Leaf paths consume all remaining key nibbles
- Extension paths are non-empty and do not directly chain to another Extension
- Branch nodes are canonical only with at least two children
- node hashes use deterministic domain-tagged encodings
- missing branch children hash as 32 zero bytes in branch encoding
- insertion order is tested by building the same 10,000-key set in two different orders and comparing roots
- updates change value commitments without increasing key count

Audit finding fixed before commit:
- Extension split failure handling originally allowed cleanup of a wrapper/direct arm to free an `old_child` still owned by the original trie if a later allocation failed. Rollback now detaches the shared old child before destroying temporary split structure.

Claims kept narrow:
- fixed-width 256-bit keys only
- no deletion or proof generation yet
- this is a teaching authenticated Patricia trie, not Ethereum wire-compatible MPT encoding

## Chapter 122 Review

Checked:
- transaction serialization is canonical big-endian, not raw C struct bytes
- append copies caller transaction buffers
- transaction Merkle roots reuse Chapter 120
- genesis previous hash is all zero
- every later block commits the exact prior block hash
- block hash commits height, prev hash, tx root, timestamp and tx count
- chain validation rebuilds Merkle roots and recomputes block hashes
- transaction proof verifies canonical transaction bytes against cached block tx root

Audit finding fixed before commit:
- capacity growth initially computed `capacity * 2` before checking whether doubling could overflow. Final grow path validates the bound first, then multiplies.

Claims kept narrow:
- no signatures, consensus, fork choice, mining/staking or network protocol
- proof generation rebuilds a block Merkle tree and is intentionally not optimized

## Chapter 123 Review

Checked:
- external 64-bit node IDs map to dense indices through open addressing
- node IDs are unique
- edges store dense endpoints
- outgoing and incoming CSR indexes contain every edge exactly once
- label index is a full node permutation sorted by (label,id)
- mutations invalidate derived index version
- queries reject stale indexes
- BFS uses dense indices and outgoing CSR
- validator cross-checks authoritative edges against both CSR directions

Audit cleanup before commit:
- label comparator control flow was expanded from compact chained statements for strict-warning readability
- index construction allocates temporary structures before publishing them

Claims kept narrow:
- fixed capacities and append-only nodes/edges in this chapter
- no MVCC, deletion or distributed partitioning

## CI Definition of Done

Batch 41 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 123 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
