# Lab 046 — Branch History

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch046_persistent_segment_tree_tests

Tasks:
1. Build [1,2,3,4].
2. Create v1 from v0 changing index 1.
3. Create v2 from v0 changing index 3.
4. Create v3 from v1.
5. Query all versions.
6. Confirm v0 never changes.
7. Compare node_count with full-copy memory.
