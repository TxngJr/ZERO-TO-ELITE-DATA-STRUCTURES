# Lab 049 — Prune Overlap Search

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch049_interval_tree_tests

Tasks:
1. Insert ten intervals.
2. Draw BST order and max_high values.
3. Query an interval that prunes the entire left subtree.
4. Query touching endpoints and verify no overlap.
5. Remove a node with two children.
6. Recompute successor path augmentation.
7. Compare randomized results with a naive scan.
