# Lab 005 — See the Call Stack

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Part A — Predict

ก่อนรัน demo:
- factorial_recursive(5) ได้อะไร
- gcd(48,18) call sequence เป็นอย่างไร
- binary search target 7 ใน sorted array จะเลือกช่วงใดบ้าง

## Run

~~~bash
./build/ch005_demo
~~~

## Part B — GDB backtrace

~~~bash
gdb ./build/ch005_demo
~~~

ตั้ง breakpoint ใน factorial_recursive:

    break factorial_recursive
    run
    continue
    bt

สังเกต active calls

## Part C — Differential tests

~~~bash
./build/ch005_recursion_tests
~~~

tests เปรียบเทียบ recursive กับ iterative results สำหรับ inputs หลายชุด

## Part D — Convert

เขียน recursive function:

    sum_digits(n)

จากนั้นแปลงเป็น while loop และเขียน invariant ของ loop

## Part E — Explicit stack design

วาด iterative DFS ของ binary tree โดยใช้ Stack ADT
ยังไม่ต้อง implement tree library แต่ต้องระบุ:
- stack เก็บอะไร
- push เมื่อไร
- pop เมื่อไร
- visited order

## Benchmark

~~~bash
./build/ch005_recursion_benchmark
~~~

ห้ามสรุปว่า recursion/iteration เร็วกว่า universally จาก benchmark นี้
