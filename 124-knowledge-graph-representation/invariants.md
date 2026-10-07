# Invariants

1. Term ID 0 is reserved for wildcard.
2. Every live term has exactly one stable dense ID.
3. Equal term strings map to the same ID.
4. Every triple references existing nonzero term IDs.
5. Duplicate triples are absent.
6. SPO/POS/OSP arrays are permutations of all triple indices.
7. SPO is sorted by (S,P,O).
8. POS is sorted by (P,O,S).
9. OSP is sorted by (O,S,P).
10. Query indexes are used only when index_version == triple_version.
