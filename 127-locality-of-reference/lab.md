# Lab — Measure Trace Geometry

1. Generate sequential trace 0..127.
2. Analyze with 8-byte elements and 64-byte lines.
3. Verify 16 unique lines and 112 same-line pairs.
4. Generate stride-8 direct trace manually.
5. Show every access moves to a new line.
6. Concatenate two sequential scans.
7. Verify second scan contributes temporal reuse, not new lines.
8. Generate a coprime modular permutation.
9. Verify every index appears once.
10. Run ASan/UBSan.

Benchmark:
- large contiguous array
- sequential traversal
- deterministic permutation traversal
- report time only as observation.
