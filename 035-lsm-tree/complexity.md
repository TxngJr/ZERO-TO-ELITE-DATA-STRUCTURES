# Complexity — Mini LSM

Teaching MemTable:
- binary lookup O(log M)
- new insert O(M) due array shift

Point read:
    O(log M + R log S)

Flush:
    Theta(M)

Full compaction implementation:
    O(N log N)
because gathered versions are sorted by key and sequence.

A production k-way merge of already-sorted runs can be closer to:

    O(N log R)

with heap merge, or specialized linear-ish merges for bounded fan-in.

Range scan here materializes/sorts for clarity, not production performance.
