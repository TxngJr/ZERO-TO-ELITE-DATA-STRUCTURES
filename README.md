# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 42 — Chapters 124–126 complete**

ล่าสุด:
- [124 Knowledge Graph Representation](./124-knowledge-graph-representation/)
- [125 Data Structure Serialization](./125-data-structure-serialization/)
- [126 Memory Alignment & Padding](./126-memory-alignment-padding/)

สถานะ: **126 / 170 chapters**

Next:

**127 Locality of Reference → 128 CPU Cache Effects → 129 False Sharing**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–126 currently document single-threaded contracts.
