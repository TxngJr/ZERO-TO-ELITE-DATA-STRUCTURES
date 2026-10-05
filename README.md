# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 21 — Chapters 061–063 complete**

ล่าสุด:
- [061 Suffix Automaton](./061-suffix-automaton/)
- [062 Aho-Corasick Automaton](./062-aho-corasick-automaton/)
- [063 Rolling Hash Structures](./063-rolling-hash-structures/)

สถานะ: **63 / 170 chapters**

Next:

**064 Bitset → 065 Bitmap → 066 Bit Vector**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
