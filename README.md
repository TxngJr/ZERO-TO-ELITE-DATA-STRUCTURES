# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 35 — Chapters 103–105 complete**

ล่าสุด:
- [103 Memory Pool / Object Pool](./103-memory-pool-object-pool/)
- [104 Arena Allocator](./104-arena-allocator/)
- [105 Slab Allocator Concepts](./105-slab-allocator-concepts/)

สถานะ: **105 / 170 chapters**

Next:

**106 Free List → 107 Garbage-Collected Data Structures → 108 Cache-Aware Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer remains scoped to concurrent Chapters 096–102 because Chapters 103–105 intentionally expose non-thread-safe allocator contracts.
