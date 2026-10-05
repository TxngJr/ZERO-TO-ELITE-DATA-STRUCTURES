# Lab 042 — Cross the Density Boundary

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch042_adaptive_graph_tests

Tasks:
1. Create V=20 graph with promote=20%, demote=10%.
2. Add edges until backend becomes Matrix.
3. Verify every edge and weight survived.
4. Remove edges until backend becomes Adj List.
5. Observe switch_count.
6. Hold density inside hysteresis band and confirm no switch.
7. Benchmark sparse -> dense -> sparse phases.
