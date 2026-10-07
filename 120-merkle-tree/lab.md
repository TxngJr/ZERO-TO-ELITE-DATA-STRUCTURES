# Lab — Build and Verify Merkle Proofs

1. Verify SHA-256 of "abc".
2. Build 4,097 fixed-size leaves.
3. Validate every stored internal hash.
4. Generate proofs for every 37th leaf.
5. Verify against root.
6. Flip one byte of a leaf and confirm proof fails.
7. Rebuild tree with one modified leaf and confirm root changes.
8. Test single-leaf tree.
9. Run ASan/UBSan.

Benchmark:
- 100,000 leaves × 32 bytes
- tree build
- 10,000 proof verifications.
