# Lab 001 — Observe Values, Calls, Pointers and Generics

## Goal

เห็นความต่างระหว่าง value, address, pointer/reference และ generic code ด้วยตัวเอง

## Step 1 — Build

จาก root repository:

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Step 2 — Predict

ก่อนรัน examples ให้เปิด source แล้วเขียนลงสมุด:

- final value ของ x คือเท่าไร
- recursive_sum(5) คืนอะไร
- pointer function เปลี่ยน caller state หรือไม่
- template max_like ถูก instantiate กับ type อะไร

## Step 3 — Run

~~~bash
./build/ch001_foundations
./build/ch001_generics
~~~

## Step 4 — Debug with GDB

~~~bash
gdb ./build/ch001_foundations
~~~

ใน gdb:

    break main
    run
    next
    print x
    print &x

สังเกตว่าค่า x และ address ของ x เป็นคนละ concept

## Step 5 — Modify

เพิ่ม struct Rectangle ที่มี width/height และ function area

จากนั้นเพิ่ม C++ template Box<T> อีกหนึ่ง instance ที่เก็บ double

## Verification

~~~bash
ctest --test-dir build --output-on-failure
~~~

## Extension

เขียน function swap สองแบบ:

- C: รับ int*
- C++: รับ int&

อธิบายว่าทำไมทั้งสองแก้ caller object ได้ แต่ type semantics ต่างกัน
