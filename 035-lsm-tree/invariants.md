# Invariants — IntLSMTree

1. MemTable keys strictly increasing
2. MemTable has at most one version per key
3. MemTable count <= configured capacity
4. every immutable run keys strictly increasing
5. each run has at most one version per key
6. runs array ordered newest to oldest
7. sequence numbers increase globally
8. first visible version in lookup order determines key state
9. logical_size equals number of visible non-tombstoned keys
10. compaction never changes logical visible state
