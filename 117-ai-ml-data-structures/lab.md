# Lab — Dataset, Mini-Batches and Embeddings

1. Build dataset 50,000 × 16.
2. Fill deterministic features and labels.
3. Create shuffled BatchPlan with batch size 128.
4. Iterate one epoch.
5. Verify each row appears exactly once.
6. Gather each batch and verify copied values.
7. Build embedding table 20,000 × 32.
8. Gather 10,000 pseudo-random IDs.
9. Verify exact embedding rows.
10. Run ASan/UBSan.

Benchmark:
- dataset 200,000 × 32
- one shuffled gather epoch
- embedding table 100,000 × 64
- 200,000 embedding gathers.
