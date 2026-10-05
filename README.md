# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 10 — Chapters 028–030 complete**

ล่าสุด:
- [028 Fibonacci Heap](./028-fibonacci-heap/)
- [029 Trie / Prefix Tree](./029-trie-prefix-tree/)
- [030 Radix Tree / Patricia Trie](./030-radix-patricia-trie/)

สถานะ: **30 / 170 chapters**

Next:

**031 Ternary Search Tree → 032 B-Tree → 033 B+ Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
