# Lab 048 — O(1) Static RMQ

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch048_sparse_table_tests

Tasks:
1. Build [8,3,6,1,7,2,5,4].
2. Draw all valid level-2 blocks.
3. Query [2,7).
4. Identify overlap of the two selected blocks.
5. Explain why overlap is safe for min.
6. Try the same overlap logic with sum and show the error.
7. Benchmark many static queries.
