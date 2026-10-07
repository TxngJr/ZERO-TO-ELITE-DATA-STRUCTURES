# Implementation Notes

Tree stores every level as a contiguous array of 32-byte hashes. Level 0 is leaves; final level contains one root.

Level counts are computed before allocation. Every `count * 32` allocation is overflow-checked.

Proofs copy sibling hashes, so they remain valid even if the tree object is later freed.

SHA-256 uses a small internal streaming context and big-endian message length as defined by SHA-256. The public `merkle_sha256` helper exists so tests can check a standard known vector independently from Merkle logic.
