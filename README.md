# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 22 — Chapters 064–066 complete**

ล่าสุด:
- [064 Bitset](./064-bitset/)
- [065 Bitmap](./065-bitmap/)
- [066 Bit Vector](./066-bit-vector/)

สถานะ: **66 / 170 chapters**

Next:

**067 Bit Trie / XOR Trie → 068 Bloom Filter → 069 Counting Bloom Filter**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
