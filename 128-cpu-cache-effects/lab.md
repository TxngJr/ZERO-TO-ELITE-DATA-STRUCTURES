# Lab — Deterministic Cache Simulation

1. Create 2-set direct-mapped cache.
2. Run lines 0,1,2,0.
3. Verify 4 misses and 2 evictions.
4. Create 1-set 2-way cache.
5. Run 0,1,0,2,1.
6. Verify exact LRU hit/miss behavior.
7. Generate 64×64 int matrix row-major trace.
8. Reset and run column-major trace.
9. Compare miss counts under same cache geometry.
10. Validate invariants.
11. Run ASan/UBSan.

Benchmark:
- millions of synthetic accesses
- compare associativity 1/2/4/8
- report simulator throughput, not hardware-cache throughput.
