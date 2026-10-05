# Lab 039 — Same Graph, Three Layouts

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch039_graph_repr_tests

Tasks:
1. Replay identical edges into all representations.
2. Compare edge existence.
3. Compare neighbor sets.
4. Remove an undirected edge and verify both physical sides.
5. Compare memory on sparse graph.
6. Repeat with denser graph.
7. Run lookup benchmark.
