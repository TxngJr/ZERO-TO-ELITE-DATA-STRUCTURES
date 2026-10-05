# Exercises — Chapter 046

## Beginner
1. What does persistence preserve?
2. What is structural sharing?
3. How many roots per version?
4. Which nodes are copied for point update?
5. Can v2 branch directly from v0 here?

## Intermediate
6. Draw three branching versions.
7. Count shared nodes.
8. Explain why old nodes must be immutable.
9. Explain arena indices vs raw pointers.
10. Trace a historical range query.

## Advanced
11. Prove old-version immutability.
12. Derive O(n+m log n) storage.
13. Compare full and partial persistence.
14. Explain rollback of unpublished nodes.
15. Discuss reclamation of unreachable versions.

## Implementation
16. Add version metadata/parent version.
17. Add persistent range-add with lazy nodes.
18. Add kth-order-statistic prefix versions.

## Challenge / Research
19. Study fat-node persistence.
20. Compare persistence with copy-on-write pages.
