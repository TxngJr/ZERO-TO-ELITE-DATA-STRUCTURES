# Batch 38 — Negative Review and Audit

## Scope

- 112 Database Index Structures
- 113 File-System Data Structures
- 114 Compiler Data Structures

## Chapter 112 Review

Checked:
- input may be unsorted
- build copies rows before sorting
- duplicate primary keys are rejected
- duplicate secondary keys are explicitly supported
- secondary ordering uses primary key as deterministic tie-breaker
- secondary entries store row indexes rather than raw pointers
- every primary row appears exactly once in secondary index
- equality and range queries preserve (secondary, primary) ordering

Claims kept narrow:
- structure is build-once/read-many and immutable after build
- it is not a transactional/MVCC database index and does not model page splits or concurrent maintenance
- logical O(log N + K) search does not imply a particular physical I/O cost

## Chapter 113 Review

Checked:
- root inode 0 is always a directory
- every created non-root inode has exactly one directory entry
- duplicate names within one parent are rejected
- file logical block count uses ceil(size/block_size)
- four direct data blocks are separated from single-indirect metadata
- crossing from four to five data blocks consumes two new physical blocks: one metadata block and one data block
- grow scans/preselects all required free blocks before committing any allocation
- shrink releases data blocks and releases indirect metadata when no longer needed
- validator detects duplicate physical references and bitmap/reference mismatches
- validator checks parent type, child ownership and per-directory name uniqueness

Claims kept narrow:
- model has no hard links, sparse holes, journaling, crash recovery or path parser
- directory lookup is linear and is not presented as production filesystem complexity

## Chapter 114 Review

Checked:
- interning returns one NameId per distinct string
- hash table load target is <= 0.5 from power-of-two capacity sizing
- current binding is stored per NameId
- declaration stores previous binding before publishing new binding
- same-scope duplicate declaration is rejected while nested shadowing is allowed
- leave scope restores previous bindings while popping symbols
- validator reconstructs binding history and compares every current binding
- def-use edges prepend to per-definition lists and next indices strictly decrease
- use-count metadata is checked against full adjacency traversal

Claims kept narrow:
- expected O(1) symbol lookup depends on open-addressing hash behavior
- symbols are valid only while their lexical scope remains active
- this chapter does not claim to implement a full AST, CFG, SSA construction or optimizer

## Static Audit Notes

- Chapter 113 uses preselection instead of rollback to avoid half-mutated allocation state on out-of-space failure.
- Chapter 114 def-use validator requires edge next indices to point only to older edges, preventing cycles in the arena-backed adjacency list.
- All three chapters remain intentionally single-threaded; TSan scope stays Chapters 096–102.

## Definition of Done

Batch 38 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 114 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
