# Lab — Tiled Locality

1. Create blocked matrix 257×193 with tile 16×16.
2. Apply 100,000 random updates.
3. Keep a dense reference.
4. Copy every logical cell back and compare.
5. Compare row-order and tile-order sums.
6. Transpose into dense output and verify all cells.
7. Validate padded edge cells remain zero.
8. Benchmark larger matrices with several tile sizes.

อย่ารายงาน benchmark เป็น universal truth; บันทึก CPU/compiler/build flags/tile size/workload ด้วย.
