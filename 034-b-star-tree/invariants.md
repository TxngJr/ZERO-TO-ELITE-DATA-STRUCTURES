# Invariants — IntBStarTree

Ordering:
1. keys inside node strictly increasing
2. child intervals obey promoted separators
3. unique keys

Shape:
4. all leaves at same depth
5. internal node with k keys has k+1 children

Capacity:
6. count <= m-1 after public operation
7. ordinary non-root nodes have count >= 2m/3 - 1
8. root occupancy is relaxed
9. children of a one-key root may use root-split occupancy exception

Tree:
10. total key count == size

Temporary overflow count=m is allowed only inside insertion before repair.
