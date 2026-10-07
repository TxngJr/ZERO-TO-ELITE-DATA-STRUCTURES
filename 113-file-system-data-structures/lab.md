# Lab — Inodes, Direct and Indirect Blocks

1. Create filesystem model with 2,048 inodes and 20,000 blocks.
2. Create 10 directories under root.
3. Create 1,000 files.
4. Resize files across 0..20 data blocks.
5. Verify direct vs indirect mapping.
6. Lookup names from parent directories.
7. Shrink half the files to zero.
8. Run full validator.
9. Compare allocated-block count before/after shrink.
10. Run ASan/UBSan.
