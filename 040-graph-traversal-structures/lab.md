# Lab 040 — Same BFS, Different Storage

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch040_graph_traversal_tests

Tasks:
1. Build one graph in all three Chapter 039 representations.
2. BFS from the same source.
3. Compare visited sets and depths.
4. Compare exact parent/order arrays and explain any differences.
5. Reconstruct a BFS path.
6. Run DFS and compare traversal-tree depth with BFS distance.
7. Benchmark sparse and dense cases.
