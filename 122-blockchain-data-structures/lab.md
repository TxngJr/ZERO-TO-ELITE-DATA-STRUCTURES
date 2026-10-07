# Lab — Hash-Linked Blocks

1. Generate 200 blocks.
2. Put 64 deterministic transactions in each block.
3. Validate whole chain.
4. Check every block height and previous hash.
5. Create transaction proofs across many blocks.
6. Modify a proof input transaction and verify failure.
7. Mutate original caller array after append and prove stored data unchanged.
8. Run ASan/UBSan.

Benchmark:
- 1,000 blocks × 32 transactions
- full validation
- 1,000 transaction proofs.
