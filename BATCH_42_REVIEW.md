# Batch 42 — Negative Review and Audit

## Scope

- 124 Knowledge Graph Representation
- 125 Data Structure Serialization
- 126 Memory Alignment & Padding

## Chapter 124 Review

Checked:
- term IDs start at 1 so 0 can safely mean wildcard
- duplicate lexical terms return the original stable ID
- triples reference only existing term IDs
- duplicate triples are rejected without changing triple count/version
- SPO/POS/OSP arrays store authoritative triple indices rather than copies
- mutation invalidates index version
- bound subject/predicate/object queries choose SPO/POS/OSP respectively
- binary search restricts the first-key range before residual filtering
- validator checks all three index permutations and sort ordering

Audit finding fixed before commit:
- the first 100,000-triple fixture used `ids[2000+i]`, which exceeded the 10,000-term table. The final generator uses 1,000 subjects × 100 predicates as unique pairs and maps object IDs into the valid 8,000-term object range. Object-query expected cardinality is computed from an independent reference loop rather than assumed.

Claims kept narrow:
- single-threaded in-memory teaching triple store
- no SPARQL parser, RDF datatype semantics, inference or distributed execution

## Chapter 125 Review

Checked:
- wire format is independent of `sizeof(DsRecord)`
- all wire integers have explicit widths and big-endian byte order
- signed values preserve bits via `memcpy` to/from uint64 containers
- parser validates magic/version/header size/record size before allocation
- record-count multiplication and total-length addition are overflow checked
- payload length must exactly equal count × 20
- trailing bytes are rejected
- CRC32 known vector is tested
- round-trip reserialization must be byte-for-byte identical

Claims kept narrow:
- CRC32 is corruption detection, not authentication
- format version 1 has fixed-size records and no schema evolution layer yet

## Chapter 126 Review

Checked:
- alignments must be nonzero powers of two
- align-up checks addition overflow before masking
- natural field offsets are aligned and non-overlapping
- final struct size is rounded to maximum field alignment
- padding count includes internal plus tail padding
- aligned-array stride is rounded from element size
- raw allocation pointer is retained separately from aligned base
- every element base is checked against requested alignment
- logical allocation/stride multiplication and allocator slack addition are overflow checked

Audit finding fixed before commit:
- one negative-test field initializer used an unnecessary nested brace form; it was changed to the direct `{1,3}` struct initializer before CI.

Claims kept narrow:
- over-alignment is a storage trade-off, not a universal performance optimization
- cache behavior and false sharing are deliberately deferred to Chapters 127–129

## CI Definition of Done

Batch 42 is complete only when:
1. Fedora ASan/UBSan full repository suite through Chapter 126 passes.
2. Existing Fedora TSan suite for Chapters 096–102 remains green.
3. README, state, coverage, roadmap, glossary and changelog are updated.
