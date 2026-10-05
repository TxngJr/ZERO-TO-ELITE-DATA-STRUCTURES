# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้นการเรียนแบบ **Predict → Build → Run → Observe → Measure → Explain**.

> เป้าหมายของ repository นี้ไม่ใช่การจำชื่อ Data Structure แต่คือการเข้าใจ abstraction, memory representation, invariants, correctness, complexity, implementation, testing, performance และ trade-offs จนสามารถสร้าง library ของตัวเองได้

## Current Progress

**Batch 04 — Chapters 010–012 complete**

1. [Chapter 001 — Programming Foundations](./001-programming-foundations/)
2. [Chapter 002 — Memory Fundamentals](./002-memory-fundamentals/)
3. [Chapter 003 — Abstract Data Type (ADT)](./003-abstract-data-type/)
4. [Chapter 004 — Complexity Analysis](./004-complexity-analysis/)
5. [Chapter 005 — Recursion & Iteration](./005-recursion-iteration/)
6. [Chapter 006 — Arrays](./006-arrays/)
7. [Chapter 007 — Strings](./007-strings/)
8. [Chapter 008 — Linked Lists](./008-linked-lists/)
9. [Chapter 009 — Stack](./009-stack/)
10. [Chapter 010 — Queue](./010-queue/)
11. [Chapter 011 — Deque](./011-deque/)
12. [Chapter 012 — Priority Queue](./012-priority-queue/)

สถานะหลักสูตร: **12 / 170 chapters**

Next: **013 Hashing Fundamentals → 014 Hash Table → 015 Hash Set / Hash Map**

## Repository Guide

- [COURSE_GUIDE.md](./COURSE_GUIDE.md)
- [ROADMAP.md](./ROADMAP.md)
- [STUDY_ORDER.md](./STUDY_ORDER.md)
- [COURSE_STATE.md](./COURSE_STATE.md)
- [COVERAGE_MATRIX.md](./COVERAGE_MATRIX.md)
- [GLOSSARY.md](./GLOSSARY.md)
- [CHANGELOG.md](./CHANGELOG.md)

## Build on Fedora

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
1. structure/concept นี้แก้ปัญหาอะไร
2. abstraction/interface คืออะไร
3. representation ใน memory เป็นอย่างไร
4. operations ทำงานอย่างไร
5. invariants คืออะไร
6. complexity มาจากไหน
7. correctness/test อย่างไร
8. production trade-offs คืออะไร

## License / References

เนื้อหาเขียนเพื่อการเรียนรู้และ paraphrase จากแนวคิดมาตรฐาน ไม่คัดลอกข้อความยาวจากแหล่งลิขสิทธิ์
