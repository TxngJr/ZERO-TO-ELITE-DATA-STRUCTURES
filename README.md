# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 29 — Chapters 085–087 complete**

ล่าสุด:
- [085 Piece Table](./085-piece-table/)
- [086 Inverted Index](./086-inverted-index/)
- [087 Posting List](./087-posting-list/)

สถานะ: **87 / 170 chapters**

Next:

**088 Sparse Matrix Representations → 089 Matrix/Tensor Storage Layout → 090 Compressed Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
