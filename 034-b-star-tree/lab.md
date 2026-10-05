# Lab 034 — Redistribute Before You Split

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch034_bstar_tests

Tasks:
1. Create order-6 tree.
2. Insert sequential keys until first root split.
3. Continue until a child overflow can be absorbed by sibling.
4. Continue until a 2-to-3 split occurs.
5. Validate leaf depth and occupancy after every insert.
6. Remove keys and explain why teaching implementation rebuilds.
7. Compare node count/height against ordinary B-Tree at similar order.
