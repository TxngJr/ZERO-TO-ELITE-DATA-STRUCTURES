# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้นการเรียนแบบ **Predict → Build → Run → Observe → Measure → Explain**.

> เป้าหมายของ repository นี้ไม่ใช่การจำชื่อ Data Structure แต่คือการเข้าใจ abstraction, memory representation, invariants, correctness, complexity, implementation, testing, performance และ trade-offs จนสามารถสร้าง library ของตัวเองได้

## Current Progress

**Batch 03 — Chapters 007–009 complete**

1. [Chapter 001 — Programming Foundations](./001-programming-foundations/)
2. [Chapter 002 — Memory Fundamentals](./002-memory-fundamentals/)
3. [Chapter 003 — Abstract Data Type (ADT)](./003-abstract-data-type/)
4. [Chapter 004 — Complexity Analysis](./004-complexity-analysis/)
5. [Chapter 005 — Recursion & Iteration](./005-recursion-iteration/)
6. [Chapter 006 — Arrays](./006-arrays/)
7. [Chapter 007 — Strings](./007-strings/)
8. [Chapter 008 — Linked Lists](./008-linked-lists/)
9. [Chapter 009 — Stack](./009-stack/)

สถานะหลักสูตร: **9 / 170 chapters**

Next: **010 Queue → 011 Deque → 012 Priority Queue**

## Repository Guide

- [COURSE_GUIDE.md](./COURSE_GUIDE.md) — วิธีเรียนและ environment
- [ROADMAP.md](./ROADMAP.md) — แผน 170 chapters
- [STUDY_ORDER.md](./STUDY_ORDER.md) — ลำดับการเรียน
- [COURSE_STATE.md](./COURSE_STATE.md) — สถานะล่าสุด
- [COVERAGE_MATRIX.md](./COVERAGE_MATRIX.md) — coverage ของแต่ละบท
- [GLOSSARY.md](./GLOSSARY.md) — ศัพท์สำคัญ
- [CHANGELOG.md](./CHANGELOG.md) — สิ่งที่เพิ่มในแต่ละ Batch

## Environment

หลักสูตรออกแบบให้ใช้ได้ดีบน Fedora Linux / x86-64 และเหมาะกับเครื่องอย่าง Acer Aspire 7 A715-43G.

ช่วงต้นใช้ C/C++ เป็นแกนเพื่อให้เห็น memory, ownership, pointer, allocation และ representation จริง ก่อนขยายไปภาษาอื่นใน chapters เฉพาะภาษา

## Build

    sudo dnf install gcc gcc-c++ cmake make gdb valgrind

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizer build:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure

## Learning Contract

ทุกบทพยายามตอบ:
1. concept/structure นี้แก้ปัญหาอะไร
2. abstraction/interface คืออะไร
3. representation ใน memory เป็นอย่างไร
4. operations ทำงานอย่างไร
5. invariants คืออะไร
6. complexity มาจากไหน
7. correctness/test อย่างไร
8. production trade-offs คืออะไร

## License / References

เนื้อหาเขียนเพื่อการเรียนรู้และ paraphrase จากแนวคิดมาตรฐาน ไม่คัดลอกข้อความยาวจากแหล่งลิขสิทธิ์
