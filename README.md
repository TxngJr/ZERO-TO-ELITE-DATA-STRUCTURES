# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 11 — Chapters 031–033 complete**

ล่าสุด:
- [031 Ternary Search Tree](./031-ternary-search-tree/)
- [032 B-Tree](./032-b-tree/)
- [033 B+ Tree](./033-b-plus-tree/)

สถานะ: **33 / 170 chapters**

Next:

**034 B* Tree → 035 LSM Tree → 036 Skip List**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
