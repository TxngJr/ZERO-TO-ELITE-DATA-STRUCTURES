# Chapter 026 — D-ary Heap

## Goal

D-ary Heap generalizes Binary Heap ให้แต่ละ node มีได้ถึง d children.

เมื่อ d เพิ่ม:
- tree height ลด
- sift-up ใช้ levels น้อยลง
- sift-down ต้อง scan children มากขึ้นต่อ level

บทนี้สร้าง dynamic integer D-ary Min-Heap ที่กำหนด d ตอน create.

## Index Formulas

สำหรับ d>=2:

    parent(i) = (i-1)/d

children ของ i เริ่มที่:

    first = d*i + 1

และมีได้ถึง d slots:

    first ... first+d-1

Implementation ต้องระวัง size_t overflow ก่อนคำนวณ d*i+1.

## Height

Complete d-ary tree มี node capacity โตประมาณ d^h.

ดังนั้น:

    h = Theta(log_d n)

เมื่อ d มากขึ้น height ลดลง.

## Push

Append แล้ว sift-up.

แต่ละ level compare กับ parent หนึ่งครั้ง:

    O(log_d n)

## Pop Min

ย้าย last ไป root แล้ว sift-down.

แต่ละ levelต้องหา smallest จาก children สูงสุด d ตัว:

    O(d log_d n)

จึงมี trade-off:
- d ใหญ่ -> fewer levels
- แต่ more comparisons per downward level

## Build-Heap

Bottom-up เช่น Binary Heap:
เริ่มจาก last internal node แล้ว sift-down ย้อนขึ้น root.

สำหรับ d>=2 aggregate work remains linear:

    Theta(n)

แนวคิดคือ nodes ที่มี height มากมีจำนวนลดแบบ geometric d^h และ cost ต่อ levelมี factor d ที่ถูกชดเชยด้วยจำนวน nodes ที่ลดลง.

## Binary Heap Is d=2

Chapter 025 คือ special case:

    d=2

D-ary implementation ควรให้ pop sequence เดียวกันในเชิงค่ากับ Binary Min-Heap แม้ internal array shapeต่างกัน.

## Choosing d

Small d:
- deeper
- fewer child comparisons

Large d:
- shallower
- more child scans
- root has many children
- potentially better/worse cache behavior depending workload and element size

Common engineering choiceเช่น 4-ary heap can reduce height while keeping child scan manageable.

ไม่มี d ที่ดีที่สุด universal.

## Decrease-Key Perspective

Dijkstra-like workloads:
- many decrease-key operations favor shallower upward paths
- extract-min still pays d-way child selection

นี่เป็นเหตุผลหนึ่งที่ branching factor อาจถูก tune ตาม workload.

## Cache Considerations

Children of a node are contiguous in array:

    d*i+1 ... d*i+d

Larger d lets one downward step scan a contiguous block.

If that block fits well in cache line(s), practical behavior may improve.

But too-large d increases comparisons and bandwidth.

## Complexity

| Operation | D-ary Heap |
|---|---:|
| peek-min | Theta(1) |
| push | O(log_d n) |
| pop-min | O(d log_d n) |
| build | Theta(n) |
| validate | Theta(n) |
| storage | Theta(n) |

## Files

- src/int_dary_heap.*
- tests/test_dary_heap.c
- examples/dary_heap_demo.c
- benchmarks/dary_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
