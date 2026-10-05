# Lab 030 — Split and Compress Paths

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch030_radix_tests

Tasks:
1. Insert carpet.
2. Insert carbon and trace split at "car".
3. Insert car and observe terminal split boundary.
4. Query prefix ending inside a compressed edge.
5. Delete carbon and inspect recompression.
6. Verify lexicographic traversal.
7. Compare prefix workload with ordinary Trie.
