# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 38 — Chapters 112–114 complete**

ล่าสุด:
- [112 Database Index Structures](./112-database-index-structures/)
- [113 File-System Data Structures](./113-file-system-data-structures/)
- [114 Compiler Data Structures](./114-compiler-data-structures/)

สถานะ: **114 / 170 chapters**

Next:

**115 Operating-System Data Structures → 116 Networking Data Structures → 117 AI/ML Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–114 currently document single-threaded contracts.
