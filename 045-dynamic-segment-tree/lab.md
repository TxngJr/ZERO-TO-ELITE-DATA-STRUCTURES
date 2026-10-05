# Lab 045 — Huge Universe, Few Points

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch045_dynamic_segment_tree_tests

Tasks:
1. Create domain [0,10^18).
2. Add values at five distant coordinates.
3. Observe node_count.
4. Query narrow and broad ranges.
5. Add negative delta returning one point to zero.
6. Observe pruning.
7. Compare memory conceptually with a dense static tree.
