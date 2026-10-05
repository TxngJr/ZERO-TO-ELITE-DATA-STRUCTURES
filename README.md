# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้นการเรียนแบบ **Predict → Build → Run → Observe → Measure → Explain**.

> เป้าหมายของ repository นี้ไม่ใช่การจำชื่อ Data Structure แต่คือการเข้าใจ abstraction, memory representation, invariants, correctness, complexity, implementation, testing, performance และ trade-offs จนสามารถสร้าง library ของตัวเองได้

## Current Progress

**Batch 02 — Chapters 004–006 complete**

1. [Chapter 001 — Programming Foundations](./001-programming-foundations/)
2. [Chapter 002 — Memory Fundamentals](./002-memory-fundamentals/)
3. [Chapter 003 — Abstract Data Type (ADT)](./003-abstract-data-type/)
4. [Chapter 004 — Complexity Analysis](./004-complexity-analysis/)
5. [Chapter 005 — Recursion & Iteration](./005-recursion-iteration/)
6. [Chapter 006 — Arrays](./006-arrays/)

สถานะหลักสูตร: **6 / 170 chapters**

Next: **007 Strings → 008 Linked Lists → 009 Stack**

## Repository Guide

- [COURSE_GUIDE.md](./COURSE_GUIDE.md) — วิธีเรียนและ environment
- [ROADMAP.md](./ROADMAP.md) — แผน 170 chapters
- [STUDY_ORDER.md](./STUDY_ORDER.md) — ลำดับการเรียน
- [COURSE_STATE.md](./COURSE_STATE.md) — สถานะล่าสุด
- [COVERAGE_MATRIX.md](./COVERAGE_MATRIX.md) — coverage ของแต่ละบท
- [GLOSSARY.md](./GLOSSARY.md) — ศัพท์สำคัญ

## Environment

หลักสูตรออกแบบให้ใช้ได้ดีบน Fedora Linux / x86-64 และเหมาะกับเครื่องอย่าง Acer Aspire 7 A715-43G.

Batch 01–02 ใช้ C/C++ เป็นแกนหลักเพื่อให้เห็น memory, pointers, templates, call stack, arrays และ dynamic allocation จริง ก่อนขยายไป Python/Rust/Java/Go ใน chapters ภาษาเฉพาะ

## Build

~~~bash
sudo dnf install gcc gcc-c++ cmake make gdb valgrind

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
~~~

Sanitizer build:

~~~bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
~~~

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
