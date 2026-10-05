# Lab 041 — Dependency Scheduler Structure

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch041_dag_tests

Tasks:
1. Create a build dependency DAG.
2. Inspect indegrees.
3. List sources/sinks.
4. Produce topological order.
5. Attempt an edge that closes a cycle.
6. Remove a prerequisite and verify indegree.
7. Explain how Kahn queue models ready tasks.
