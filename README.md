# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 36 — Chapters 106–108 complete**

ล่าสุด:
- [106 Free List](./106-free-list/)
- [107 Garbage-Collected Data Structures](./107-garbage-collected-data-structures/)
- [108 Cache-Aware Data Structures](./108-cache-aware-data-structures/)

สถานะ: **108 / 170 chapters**

Next:

**109 Cache-Oblivious Data Structures → 110 External-Memory Data Structures → 111 Disk-Based Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–108 currently document single-threaded contracts.
