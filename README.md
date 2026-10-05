# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 14 — Chapters 040–042 complete**

ล่าสุด:
- [040 Graph Traversal Structures](./040-graph-traversal-structures/)
- [041 DAG Data Structures](./041-dag-data-structures/)
- [042 Sparse vs Dense Graph Representation](./042-sparse-vs-dense-graph-representation/)

สถานะ: **42 / 170 chapters**

Next:

**043 Segment Tree → 044 Lazy Propagation Segment Tree → 045 Dynamic Segment Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
