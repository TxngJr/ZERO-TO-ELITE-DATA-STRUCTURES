# Lab — Authenticated Compressed Trie

1. Generate 10,000 fixed 32-byte keys.
2. Insert ascending order.
3. Insert same set in shuffled order into second trie.
4. Compare roots.
5. Verify all lookups.
6. Update every 10th value and confirm root changes.
7. Validate structure/hash semantics.
8. Run ASan/UBSan.

Benchmark:
- 50,000 inserts
- 200,000 exact lookups
- root recomputation.
