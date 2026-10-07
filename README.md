# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 40 — Chapters 118–120 complete**

ล่าสุด:
- [118 Vector Search Structures](./118-vector-search-structures/)
- [119 Game Data Structures](./119-game-data-structures/)
- [120 Merkle Tree](./120-merkle-tree/)

สถานะ: **120 / 170 chapters**

Next:

**121 Merkle Patricia Trie → 122 Blockchain Data Structures → 123 Graph Database Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–120 currently document single-threaded contracts.
