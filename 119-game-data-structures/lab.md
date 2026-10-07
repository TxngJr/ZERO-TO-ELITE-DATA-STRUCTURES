# Lab — ECS Identity + Spatial Query

1. Spawn 50,000 entities.
2. Attach positions on deterministic 500×100 layout.
3. Destroy one entity and immediately reuse its slot.
4. Prove stale EntityId fails.
5. Rebuild a 10-unit spatial grid.
6. Query AABB and verify 1,250 exact matches.
7. Mutate world and prove stale-grid query is rejected.
8. Rebuild and validate again.
9. Run ASan/UBSan.

Benchmark:
- 100,000 entities
- repeated full grid rebuilds
- 10,000 AABB queries.
