# Lab 038 — One Graph, Clear Semantics

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch038_graph_tests

Tasks:
1. Build an undirected 6-vertex graph with one isolated vertex.
2. Compute every degree.
3. Verify sum degrees = 2E.
4. Build directed version.
5. Verify in/out-degree sums.
6. Attempt duplicate/self-loop insertion.
7. Measure edge-list lookup as E grows.
