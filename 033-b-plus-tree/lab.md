# Lab 033 — Range Scan Across Leaves

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch033_bplus_tests

Tasks:
1. Insert 1..40 with t=2.
2. Draw leaf chain after splits.
3. Search keys equal to internal separators.
4. Range scan [13,31].
5. Delete first key of a leaf and watch parent separator update.
6. Trigger leaf borrow and merge.
7. Compare range scan against B-Tree inorder filtering.
