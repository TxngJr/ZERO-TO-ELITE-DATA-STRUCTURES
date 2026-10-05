# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 23 — Chapters 067–069 complete**

ล่าสุด:
- [067 Bit Trie / XOR Trie](./067-bit-trie-xor-trie/)
- [068 Bloom Filter](./068-bloom-filter/)
- [069 Counting Bloom Filter](./069-counting-bloom-filter/)

สถานะ: **69 / 170 chapters**

Next:

**070 Cuckoo Filter → 071 Cuckoo Hashing → 072 Perfect Hashing**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
