# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 26 — Chapters 076–078 complete**

ล่าสุด:
- [076 Count-Min Sketch](./076-count-min-sketch/)
- [077 Count Sketch](./077-count-sketch/)
- [078 Reservoir Sampling Structures](./078-reservoir-sampling-structures/)

สถานะ: **78 / 170 chapters**

Next:

**079 Linked Hash Map → 080 Ordered Map / Ordered Set → 081 Multiset / Multimap**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
