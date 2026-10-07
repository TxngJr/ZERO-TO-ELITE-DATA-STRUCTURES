# Lab — Indexed Property Graph

1. Add 20,000 nodes with 25 labels.
2. Add one type-1 chain edge between consecutive nodes.
3. Add 80,000 deterministic type-2 edges.
4. Build indexes.
5. Verify each label has 800 nodes.
6. Verify outgoing/incoming type-1 neighbors.
7. Run BFS from node 0 to node 999; expect 999 hops.
8. Add an edge and prove queries reject stale indexes.
9. Rebuild and validate every index.
10. Run ASan/UBSan.

Benchmark:
- 50,000 nodes
- ~250,000 edges
- full index build
- 10,000 neighbor queries
- repeated label queries.
