# Course Guide

## วิธีใช้หลักสูตรนี้

หลักสูตรนี้ใช้แนวคิด Predict → Build → Run → Observe → Measure → Explain

ก่อนรันตัวอย่างทุกครั้งให้ลองตอบก่อนว่าโปรแกรมจะทำอะไร จากนั้นจึง compile/run และเปรียบเทียบผลกับสิ่งที่คาดไว้

## Fedora setup

ติดตั้งเครื่องมือพื้นฐาน:

~~~bash
sudo dnf install gcc gcc-c++ cmake make gdb valgrind git
~~~

ตรวจสอบ:

~~~bash
gcc --version
g++ --version
cmake --version
gdb --version
~~~

Build ทั้ง Batch 01:

~~~bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
~~~

เปิด sanitizer build:

~~~bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
~~~

## วิธีเรียนหนึ่ง Chapter

1. อ่าน README เพื่อสร้าง mental model
2. อ่าน theory.md เพื่อเข้าใจรายละเอียดและคำศัพท์
3. พิมพ์/แก้ code เอง อย่า copy อย่างเดียว
4. ทำ lab.md ตามลำดับ
5. ทำ exercises.md โดยยังไม่เปิดเฉลยจากแหล่งอื่น
6. ทำ quiz.md
7. ตรวจว่าตัวเองอธิบาย concept โดยไม่เปิดโน้ตได้หรือไม่

## Language strategy

C ใช้เรียน pointer, memory layout และ manual allocation
C++ ใช้เรียน references, class, templates และ generic containers
Python/Rust/Java/Go จะถูกใช้เพื่อเปรียบเทียบ model ของภาษาและลงลึกใน chapters 157–162

## กติกาสำคัญ

- อย่าจำ Big-O โดยไม่เข้าใจว่าจำนวน operation โตอย่างไร
- อย่าจำ pointer syntax โดยไม่วาด address diagram
- อย่าเรียก implementation ว่า ADT: ADT คือสัญญาเชิงพฤติกรรม ส่วน implementation คือวิธีทำให้สัญญานั้นเกิดขึ้น
- code ที่มี undefined behavior ไม่ถือว่า "ทำงานได้" แม้จะดูเหมือนให้ผลถูกในบางครั้ง
