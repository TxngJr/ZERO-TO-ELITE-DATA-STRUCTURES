# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 43 — Chapters 127–129 complete**

ล่าสุด:
- [127 Locality of Reference](./127-locality-of-reference/)
- [128 CPU Cache Effects](./128-cpu-cache-effects/)
- [129 False Sharing](./129-false-sharing/)

สถานะ: **129 / 170 chapters**

Next:

**130 Pointer Chasing → 131 Amortized Data Structures → 132 Randomized Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer covers concurrent Chapters 096–102 plus Chapter 129's atomic false-sharing workload.
