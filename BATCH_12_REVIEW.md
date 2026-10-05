# Batch 12 — Negative Review and Audit

## Scope

- 034 B* Tree
- 035 LSM Tree
- 036 Skip List

## B* Tree Review

Checked:
- implementation is not mislabeled ordinary B-Tree
- sibling redistribution is attempted before allocation/split
- two-node overflow uses 2-to-3 split
- parent separator participates in redistribution
- order convention is explicit and restricted to multiples of 3
- root split has a documented occupancy exception
- validator checks ordering, occupancy, child intervals and equal leaf depth
- teaching deletion does not reuse ordinary B-Tree deletion and silently violate 2/3 occupancy
- remove uses replacement-tree rebuild and is documented O(n log n), not O(log n)

Failure-safety design:
insertion preallocates spare nodes and temporary merge buffers before mutation, avoiding allocation failure after a child has already overflowed.

## LSM Review

Checked:
- MemTable and each run are sorted/unique by key
- point lookup order is MemTable then newest run to oldest run
- tombstone stops lookup and shadows old versions
- sequence numbers are globally increasing
- flush copies MemTable before clearing it
- compaction builds replacement versions before freeing old runs
- full immutable-run compaction keeps newest sequence per key
- tombstones are dropped only because every older immutable run participates
- logical_size is cross-checked against materialized visible state
- teaching range scan/materialized compaction complexity is not represented as production-optimal

Durability warning:
the chapter explicitly does not claim in-memory runs are crash-safe SSTables and identifies WAL/manifest/checksum work as later-course material.

## Skip List Review

Checked:
- level 0 contains every node in strict key order
- higher levels are strict ordered subsets
- flexible-array allocation matches sampled node height
- insert/delete build predecessor update arrays
- deletion unlinks every participating level
- current_level shrinks after top levels empty
- validator checks upper-level nodes also exist at level 0
- deterministic PRNG is described as reproducibility tool, not cryptographic randomness
- expected O(log n) is not misstated as deterministic worst-case

## Testing Review

- B* randomized differential: 12,000 operations
- LSM randomized workload: 25,000 operations
- Skip List randomized differential: 40,000 operations
- structure validators run continuously after mutations
- B* multiple orders 3/6/9/12
- LSM scan before/after compaction compared exactly
- Skip List range output compared with boolean reference set

## Complexity Contrast

B*:
- fixed-order point/update path O(log n)
- teaching deletion O(n log n)

LSM:
- buffered write path
- reads pay run amplification
- compaction pays rewrite cost

Skip List:
- expected O(log n)
- worst O(n)

## Definition of Done

Batch 12 is complete only when Fedora CI configures, builds Chapters 001–036 with warnings + ASan/UBSan and passes complete CTest.
