# Chapter 004 — Complexity Analysis

## Goal

ตั้งแต่บทนี้เป็นต้นไป เราจะไม่พูดว่า Data Structure "เร็ว" หรือ "ช้า" แบบลอย ๆ แต่จะกำหนด input size, cost model, case และวิเคราะห์ว่าปริมาณงาน/หน่วยความจำเติบโตอย่างไรเมื่อ input โตขึ้น

หลังจบบทนี้คุณควร:

- แยก exact operation count ออกจาก asymptotic growth
- ใช้ Big-O, Big-Ω และ Big-Θ อย่างถูกความหมาย
- เข้าใจ little-o และ little-ω ในฐานะ strict asymptotic bounds
- แยก best / average / worst / amortized case
- วิเคราะห์ single loop, nested loop, halving loop และ recursion ง่าย ๆ
- แยก total space กับ auxiliary space
- ใช้ logarithm อธิบาย repeated halving/doubling
- อธิบาย amortized analysis ด้วย aggregate, accounting และ potential method ระดับพื้นฐาน
- รู้ว่า Big-O ไม่แทน benchmark, cache behavior หรือ constant factors

## 1. Complexity เริ่มจากคำถามว่า n คืออะไร?

ก่อนเขียน O(...) ต้องระบุ input size ก่อน

ตัวอย่าง:
- array search: n = จำนวน elements
- graph: มักใช้ V = vertices, E = edges
- string search: อาจมี n = text length และ m = pattern length
- hash table: n = number of stored entries และ load factor ก็สำคัญ

ถ้าไม่กำหนด n ประโยค "algorithm นี้ O(n)" ยังไม่สมบูรณ์

## 2. Cost model

เรามักนับ primitive operations ภายใต้ model ที่กำหนด เช่น comparison, array access, assignment หรือ arithmetic บน machine word

นี่เป็น model เพื่อ reasoning ไม่ใช่การบอกว่าแต่ละ operation ใช้ nanoseconds เท่ากันจริง

## 3. Big-O, Big-Ω, Big-Θ

ให้ f(n) เป็น cost จริงเชิงจำนวน operation และ g(n) เป็น growth function

Big-O คือ asymptotic upper bound:

    f(n) ∈ O(g(n))

ถ้ามี c > 0 และ n0 ที่ทำให้

    0 <= f(n) <= c g(n)  สำหรับทุก n >= n0

Big-Ω คือ asymptotic lower bound:

    f(n) ∈ Ω(g(n))

Big-Θ คือ tight asymptotic bound เมื่อเป็นทั้ง O(g(n)) และ Ω(g(n))

ตัวอย่าง:

    f(n) = 3n + 7

เป็น Θ(n) และยังเป็น O(n²) ได้ด้วย แต่ Θ(n) สื่อ tight growth class ได้ดีกว่า

## 4. little-o และ little-ω

little-o หมายถึง strict upper growth:

    f(n) ∈ o(g(n))

เมื่อ f(n)/g(n) → 0

เช่น n ∈ o(n²)

little-ω คือ strict lower growth เช่น n² ∈ ω(n)

## 5. Common growth classes

โดยทั่วไปจากโตช้าสู่โตเร็ว:

    Θ(1)
    Θ(log n)
    Θ(n)
    Θ(n log n)
    Θ(n²)
    Θ(n³)
    Θ(2^n)
    Θ(n!)

## 6. Dominant term

ถ้า:

    T(n) = 8n² + 3n + 100

เมื่อ n ใหญ่ term n² ครองการเติบโต:

    T(n) ∈ Θ(n²)

นี่ไม่ได้แปลว่า constants ไม่มีผลต่อ runtime จริง แต่ asymptotic classification สนใจ growth

## 7. Single loop

    for i = 0 .. n-1:
        constant_work()

body รัน n ครั้ง จึงได้ Θ(n)

## 8. Nested and dependent loops

สอง loop ขนาด n ให้ n*n work เมื่อ inner bound เป็น n จริง

แต่ triangular loop:

    for i = 0..n-1
        for j = 0..i

มีจำนวนรอบ:

    1 + 2 + ... + n
    = n(n+1)/2
    ∈ Θ(n²)

และ loop ที่คูณ i ด้วย 2 ทุกครั้งมี Θ(log n) iterations แม้จะเป็น loop เดียว

## 9. Logarithm mental model

ถ้าหาร problem size ครึ่งหนึ่ง:

    n
    n/2
    n/4
    ...
    1

จำนวน step k ทำให้ n / 2^k = 1 จึงได้ k = log₂ n

นี่คือ intuition ของ binary search

## 10. Best / Average / Worst case

Linear search:
- best: target อยู่ตัวแรก → Θ(1)
- worst: target อยู่ท้ายหรือไม่มี → Θ(n)
- average: ต้องกำหนด probability model ก่อน

Average case ไม่ใช่การเดาค่ากลาง

## 11. Time / Space / Auxiliary Space

Total space รวมสิ่งที่ model กำหนดทั้งหมด
Auxiliary space คือพื้นที่เพิ่มนอกเหนือจาก input representation

Recursive algorithm อาจใช้ call stack แม้ไม่มี malloc

## 12. Amortized analysis

Dynamic array append:
- ปกติ: เขียนหนึ่ง element
- ตอนเต็ม: grow และ copy หลาย elements

append ครั้งเดียวจึงมี worst case Θ(n)

ถ้า capacity double ทุกครั้ง ค่า copy ตลอด n appends เป็น geometric series:

    1 + 2 + 4 + ... < 2n

รวมแล้ว n appends ใช้ Θ(n) work จึงได้ Θ(1) amortized ต่อ append

Amortized ไม่ใช่ average-case probability

### Aggregate method
หา total cost ของ sequence แล้วหารด้วยจำนวน operations

### Accounting method
เก็บเครดิตจาก operation ถูกไว้จ่าย operation แพง

### Potential method

    amortized cost = actual cost + Φ(after) - Φ(before)

Chapter 131 จะลงลึกอีกครั้ง

## 13. Why asymptotics are not enough

สอง implementations ที่ Θ(n) เหมือนกันอาจต่างมากเพราะ:
- constants
- cache locality
- vectorization
- branch prediction
- allocation
- pointer chasing
- compiler optimization

จึงควรใช้ทั้ง proof/analysis + benchmark + profiling

## 14. Discipline สำหรับบทถัดไป

ทุก complexity table ควรบอก:
- operation
- input size
- best/average/worst เมื่อมีความหมาย
- amortized เมื่อเกี่ยวข้อง
- time
- auxiliary space
- assumptions

## Files

- theory.md — formal reasoning เพิ่มเติม
- visual-model.md — growth และ repeated halving
- implementation.md — operation counters
- complexity.md — derivation cookbook
- invariants.md — assumptions ของ analysis
- pitfalls.md — จุดผิดที่พบบ่อย
- lab.md — count → predict → run → compare
- examples/operation_counts.c
- benchmarks/scaling.c
- tests/test_complexity.c
