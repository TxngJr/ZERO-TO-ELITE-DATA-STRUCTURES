# Lab 003 — Build and Break a Stack ADT

## Build and test

~~~bash
cmake -S . -B build -DDS_ENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
~~~

## Part A — Read interface first

เปิด src/int_stack.h โดยยังไม่ดู .c

เขียน contract ของแต่ละ function จาก signature/documentation

ตอบ:
- caller เป็นเจ้าของ IntStack pointer หรือ library
- out parameter มีความหมายเมื่อ function คืน false หรือไม่
- operation ใด mutate state

## Part B — Run tests

~~~bash
./build/ch003_stack_tests
~~~

## Part C — Inspect implementation

เปิด src/int_stack.c แล้ววาด representation:

    IntStack
    +----------+
    | data ----+----> [10][20][30][...]
    | size=3   |
    | cap=4    |
    +----------+

เขียน invariant ด้วยคำของตัวเอง

## Part D — Deliberately break invariant

สร้าง branch/local edit แล้วเปลี่ยน push ให้เพิ่ม size ก่อน ensure capacity

คิดว่า allocation failure จะเกิด state แบบใด

อย่า commit bug นี้เข้า main

## Part E — Alternative representation

ออกแบบ linked-stack representation บนกระดาษ โดยห้ามเปลี่ยน public behavior

ระบุ fields ที่ต้องมีและ ownership ของแต่ละ node

## Success criteria

- tests pass
- sanitizer ไม่รายงาน error
- อธิบายได้ว่า test suite ยังควรใช้ได้แม้เปลี่ยน representation
