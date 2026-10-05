# Lab 052 — Ask the Tree for Rank

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch052_order_stat_tree_tests

Tasks:
1. Insert 10,20,...,70 in mixed order.
2. Draw subtree_size for every node.
3. Trace select(4).
4. Trace rank(55).
5. Remove a two-child node.
6. Recompute metadata after rotations.
7. Compare rank/select with a sorted reference set.
