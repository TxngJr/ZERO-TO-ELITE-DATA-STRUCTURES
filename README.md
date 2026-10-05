# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 27 — Chapters 079–081 complete**

ล่าสุด:
- [079 Linked Hash Map](./079-linked-hash-map/)
- [080 Ordered Map / Ordered Set](./080-ordered-map-ordered-set/)
- [081 Multiset / Multimap](./081-multiset-multimap/)

สถานะ: **81 / 170 chapters**

Next:

**082 Circular Buffer / Ring Buffer → 083 Gap Buffer → 084 Rope**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
