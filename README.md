# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 39 — Chapters 115–117 complete**

ล่าสุด:
- [115 Operating-System Data Structures](./115-operating-system-data-structures/)
- [116 Networking Data Structures](./116-networking-data-structures/)
- [117 AI/ML Data Structures](./117-ai-ml-data-structures/)

สถานะ: **117 / 170 chapters**

Next:

**118 Vector Search Structures → 119 Game Data Structures → 120 Merkle Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102. Chapters 103–117 currently document single-threaded contracts.
