# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 37 — Chapters 109–111 complete**

ล่าสุด:
- [109 Cache-Oblivious Data Structures](./109-cache-oblivious-data-structures/)
- [110 External-Memory Data Structures](./110-external-memory-data-structures/)
- [111 Disk-Based Data Structures](./111-disk-based-data-structures/)

สถานะ: **111 / 170 chapters**

Next:

**112 Database Index Structures → 113 File-System Data Structures → 114 Compiler Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–111 currently document single-threaded contracts.
