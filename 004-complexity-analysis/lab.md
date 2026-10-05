# Lab 004 — Count Before You Time

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Part A — Predict

สำหรับ n=8 คำนวณจำนวน work ของ:
- constant
- linear
- triangular
- halving

จากนั้นรัน:

~~~bash
./build/ch004_operation_counts
~~~

บันทึก n=8,16,32,64 และคำนวณ ratio count(2n)/count(n)

## Part B — Benchmark

~~~bash
./build/ch004_scaling_benchmark
~~~

รันหลายครั้ง แล้วตอบ:
1. trend ตรงกับ counter หรือไม่
2. timing noise มาจากอะไรได้บ้าง
3. ทำไม n เล็กอาจไม่เรียงสวย
4. compiler optimization มีผลอย่างไร

## Part C — Proof

พิสูจน์:

    7n+20 ∈ Θ(n)

และ:

    n²+100n ∈ Θ(n²)

## Part D — Amortized notebook

capacity เริ่ม 1 และ double ทุกครั้ง
เขียน copy events ของการ append 16 elements
หา total copies และเฉลี่ยต่อ append

## Verify

~~~bash
ctest --test-dir build --output-on-failure
~~~
