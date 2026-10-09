# Lab — Packed vs Padded Atomic Counters

1. Create 8 counters, stride 8, line size 64.
2. Verify all eight occupy one modeled line.
3. Create 8 counters, stride 64.
4. Verify each counter occupies a distinct modeled line.
5. Run 4 threads × 100,000 increments on packed counters.
6. Verify every selected counter equals 100,000.
7. Repeat padded layout.
8. Reset and verify zero.
9. Run under ThreadSanitizer.
10. Run ASan/UBSan separately.

Benchmark:
- 8 worker threads
- packed stride 8
- padded stride 64
- same atomic operation count
- print timing but never make correctness depend on which wins.
