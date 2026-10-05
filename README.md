# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้นการเรียนแบบ **Predict → Build → Run → Observe → Measure → Explain**.

> เป้าหมายของ repository นี้ไม่ใช่การจำชื่อ Data Structure แต่คือการเข้าใจ abstraction, memory representation, invariants, correctness, complexity, implementation, testing, performance และ trade-offs จนสามารถสร้าง library ของตัวเองได้

## Current Progress

**Batch 01 — Chapters 001–003**

1. [Chapter 001 — Programming Foundations](./001-programming-foundations/)
2. [Chapter 002 — Memory Fundamentals](./002-memory-fundamentals/)
3. [Chapter 003 — Abstract Data Type (ADT)](./003-abstract-data-type/)

สถานะหลักสูตร: **3 / 170 chapters**

## Repository Guide

- [COURSE_GUIDE.md](./COURSE_GUIDE.md) — วิธีเรียนและ environment
- [ROADMAP.md](./ROADMAP.md) — แผน 170 chapters
- [STUDY_ORDER.md](./STUDY_ORDER.md) — ลำดับการเรียน
- [COURSE_STATE.md](./COURSE_STATE.md) — สถานะล่าสุด
- [COVERAGE_MATRIX.md](./COVERAGE_MATRIX.md) — coverage ของแต่ละบท
- [GLOSSARY.md](./GLOSSARY.md) — ศัพท์สำคัญ

## Environment

หลักสูตรออกแบบให้ใช้ได้ดีบน Fedora Linux / x86-64 และเหมาะกับเครื่องอย่าง Acer Aspire 7 A715-43G.

ภาษาหลักใน Batch 01:

- C — pointer, stack/heap, manual memory
- C++ — references, class, template/generics
- Python — high-level comparison
- Rust / Java / Go — cross-language mental model

## Build Batch 01 Examples

```bash
sudo dnf install gcc gcc-c++ cmake make gdb valgrind

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Learning Contract

ทุกบทพยายามตอบคำถาม 8 ข้อนี้:

1. Structure / concept นี้แก้ปัญหาอะไร?
2. Interface หรือ abstraction คืออะไร?
3. Representation ใน memory เป็นอย่างไร?
4. Operations ทำงานอย่างไร?
5. Invariants คืออะไร?
6. Complexity มาจากไหน?
7. จะพิสูจน์และทดสอบ correctness อย่างไร?
8. ใน production ควรใช้เมื่อไร และไม่ควรใช้เมื่อไร?

## License / References

เนื้อหาใน repository เขียนขึ้นเพื่อการเรียนรู้ โดยอ้างอิงแนวคิดจากตำราและเอกสารมาตรฐาน แต่ไม่คัดลอกข้อความยาวจากแหล่งลิขสิทธิ์
