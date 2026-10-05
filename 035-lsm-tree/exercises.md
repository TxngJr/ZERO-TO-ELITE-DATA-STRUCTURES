# Exercises — Chapter 035

## Beginner
1. What is a MemTable?
2. What is an immutable run?
3. What does tombstone mean?
4. Which version wins?
5. Why read newest first?

## Intermediate
6. Trace put/flush/get.
7. Trace deletion across old runs.
8. Explain full compaction.
9. Explain read amplification.
10. Explain write amplification.

## Advanced
11. Prove newest-sequence compaction preserves visible state.
12. Explain safe tombstone dropping.
13. Compare leveled vs size-tiered.
14. Design k-way merge scan.
15. Explain snapshot sequence implications.

## Implementation
16. Replace sorted-array MemTable with Skip List.
17. Add per-run Bloom Filter.
18. Add binary-search block index.

## Challenge / Research
19. Model leveled compaction write amplification.
20. Add WAL + manifest crash-recovery design.
