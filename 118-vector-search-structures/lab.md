# Lab — Exact k-NN Baseline

1. Build 20,000 × 8 vectors.
2. Query 100 vectors.
3. Compute independent full-sort reference.
4. Compare top-10 IDs and distances exactly.
5. Test cosine self-match.
6. Add equal-distance vectors and verify ID tie-break.
7. Run ASan/UBSan.

Benchmark:
- 50,000 vectors × 32 dims
- 200 top-10 L2 queries.
