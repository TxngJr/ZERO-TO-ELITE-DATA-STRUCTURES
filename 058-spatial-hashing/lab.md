# Lab 058 — Hash the World into Cells

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch058_spatial_hash_tests

Tasks:
1. Set cell size 10.
2. Insert points around x=0 and y=0.
3. Verify negative-cell mapping.
4. Query a rectangle touching four cells.
5. Trigger table rehash.
6. Compare with naive scan.
7. Benchmark several cell sizes.
