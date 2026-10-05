# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 13 — Chapters 037–039 complete**

ล่าสุด:
- [037 Disjoint Set / Union-Find](./037-disjoint-set-union-find/)
- [038 Graphs Fundamentals](./038-graphs-fundamentals/)
- [039 Graph Representations](./039-graph-representations/)

สถานะ: **39 / 170 chapters**

Next:

**040 Graph Traversal Structures → 041 DAG Data Structures → 042 Sparse vs Dense Graph Representation**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
