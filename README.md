# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 41 — Chapters 121–123 complete**

ล่าสุด:
- [121 Merkle Patricia Trie](./121-merkle-patricia-trie/)
- [122 Blockchain Data Structures](./122-blockchain-data-structures/)
- [123 Graph Database Structures](./123-graph-database-structures/)

สถานะ: **123 / 170 chapters**

Next:

**124 Knowledge Graph Representation → 125 Data Structure Serialization → 126 Memory Alignment & Padding**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–123 currently document single-threaded contracts.
