# Lab 006 — Build a Dynamic Array From Scratch

## Part A — Static array layout

Build:

~~~bash
cmake -S . -B build
cmake --build build
~~~

Run:

~~~bash
./build/ch006_array_layout
~~~

สังเกต addresses ของ:
- a[0], a[1], ...
- matrix rows/elements

ตอบ:
1. elements contiguous หรือไม่
2. sizeof(array) ต่างจาก sizeof(pointer) อย่างไรใน main
3. row-major สังเกตจาก address order อย่างไร

## Part B — IntVector tests

~~~bash
./build/ch006_vector_tests
~~~

เปิด test แล้วดู randomized differential section

อธิบายว่าทำไม reference model ช่วยจับ sequence bug ที่ unit tests แบบ fixed cases อาจพลาด

## Part C — Sanitizers

~~~bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
~~~

## Part D — Trace capacity

เขียนโปรแกรม push 20 values แล้วพิมพ์:
- size
- capacity
- backing pointer

หลังแต่ละ push

สังเกตว่าจุดใด capacity เปลี่ยน และ pointer เปลี่ยนหรือไม่

อย่าสรุปว่า realloc ต้องย้ายทุกครั้ง

## Part E — Benchmark

~~~bash
./build/ch006_vector_benchmark
~~~

เปรียบเทียบ:
- push back
- insert at front

เมื่อ n double วิเคราะห์ growth trend

## Part F — Modification challenge

เพิ่ม function:

    int_vector_remove_value_first

ที่ลบ occurrence แรกของ value

ก่อน code ให้เขียน:
- contract
- complexity
- invariant preservation
- tests
