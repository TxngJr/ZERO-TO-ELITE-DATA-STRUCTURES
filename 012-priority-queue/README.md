# Chapter 012 — Priority Queue

## Goal

Priority Queue ไม่ได้เอา element ออกตามอายุแบบ Queue หรือความใหม่แบบ Stack แต่เลือก element ตาม priority บทนี้จะแยก ADT ออกจาก implementation และสร้าง Max Priority Queue ด้วย Binary Heap

หลังจบบทนี้คุณควร:
- นิยาม Priority Queue ADT
- แยก FIFO queue ออกจาก priority ordering
- อธิบาย min-priority vs max-priority queue
- implement binary max-heap backed priority queue
- เข้าใจ complete binary tree ใน array
- ใช้ parent/left/right index formulas
- เข้าใจ heap invariant
- implement sift-up และ sift-down
- วิเคราะห์ push/pop/peek
- เขียน invariant validator
- ทำ randomized differential tests กับ reference model
- เข้าใจ tie behavior และ stability
- เชื่อม priority queue กับ schedulers, Dijkstra/A*, top-k และ event simulation

## 1. Priority Queue ADT

Item อาจเป็น:

    (value, priority)

Max Priority Queue:
- push(value, priority)
- peek_max()
- pop_max()
- size()
- empty()

item ที่มี priority มากสุดออกก่อน

ตัวอย่าง:

    push(A,2)
    push(B,10)
    push(C,5)

pop order:
    B, C, A

ไม่ใช่ FIFO

## 2. Tie Semantics

ถ้าสอง items priority เท่ากัน:

    (A,5), (B,5)

Priority Queue ADT ต้องบอกว่า:
- order ไม่รับประกัน
- หรือ stable: A ก่อน B ถ้า A เข้าเก่ากว่า
- หรือใช้ secondary comparator

implementation ในบทนี้ **ไม่รับประกัน stability สำหรับ equal priorities**

Tests จึงไม่สมมติ tie order

## 3. Simple Implementations

### Unsorted Array
push:
    append Θ(1) amortized

pop max:
    scan หา max Θ(n)
    remove Θ(n) ถ้ารักษาลำดับ หรือ O(1) ถ้า swap-with-last และไม่สน order

### Sorted Array
push:
    Θ(n) เพื่อรักษาลำดับ

peek/pop max:
    Θ(1) ที่ปลายที่เหมาะสม

### Binary Heap
push:
    Θ(log n)

peek max:
    Θ(1)

pop max:
    Θ(log n)

นี่คือ balanced trade-off ที่นิยม

## 4. Binary Heap Shape

Binary Heap เป็น complete binary tree:
- ทุก level เต็ม ยกเว้น level สุดท้าย
- level สุดท้ายเติมจากซ้ายไปขวา

เพราะ shape นี้จึงเก็บใน array ได้โดยไม่ต้อง pointers

index i:

    parent = (i-1)/2     when i>0
    left   = 2i+1
    right  = 2i+2

ต้องระวัง arithmetic overflow ใน code ทั่วไป; validator ของบทนี้ใช้ bounds checks ก่อนเข้าถึง children

## 5. Max-Heap Invariant

สำหรับทุก node i ที่มี children:

    priority(parent) >= priority(child)

ดังนั้น root index 0 มี priority สูงสุด

Heap ไม่ได้ sorted ทั้ง array

ตัวอย่าง valid max heap priorities:

    [100, 70, 90, 20, 60, 50]

70 < 90 ได้ เพราะเป็น siblings/subtrees คนละ branch
สิ่งที่ invariant กำหนดคือ parent-child

## 6. Push / Sift Up

1. append item ที่ท้าย array
2. ขณะที่ priority > parent priority:
   - swap กับ parent
3. หยุดเมื่อ root หรือ invariant ถูกคืน

จำนวนระดับของ complete tree:
    Θ(log n)

push:
    Θ(log n) worst

## 7. Pop Max / Sift Down

1. save root
2. ย้าย last item มา root
3. size--
4. เปรียบเทียบกับ child ที่ priority สูงกว่า
5. swap ลงจน invariant restored

pop max:
    Θ(log n)

peek:
    root โดยไม่แก้ structure → Θ(1)

## 8. Why Height is Logarithmic

Complete binary tree ที่ height h เก็บ nodes ได้ประมาณ exponential ใน h

    1 + 2 + 4 + ... + 2^h

ดังนั้น height เป็น Θ(log n)

Chapter 025 จะลงลึก Heap มากขึ้น รวม heap construction/heap sort และ variants

## 9. Heap vs Sorted Structure

Priority Queue ต้องการ "best item now" ไม่จำเป็นต้อง iterate sorted ทั้งหมด

ถ้าต้อง:
- predecessor/successor
- arbitrary ordered iteration
- range query

ordered tree อาจเหมาะกว่า

ถ้าต้อง repeatedly extract best:
heap มักเป็น choice ที่ดี

## 10. Stability

Heap swap operations สามารถเปลี่ยน relative order ของ equal-priority items

Stable priority queue สามารถเพิ่ม:
- monotonic sequence number
- comparator (priority, sequence)

แต่มี metadata และ overflow/ordering considerations เพิ่ม

## 11. Increase / Decrease Key Preview

Graph algorithms บางแบบต้องปรับ priority ของ item ที่อยู่ใน queue

ถ้าต้อง find arbitrary item:
heap อย่างเดียวไม่มี Θ(1) lookup by value

แนวทาง:
- external map value→heap index
- lazy insertion of new priority and ignore stale entries
- indexed priority queue

จะกลับมาเมื่อเรียน graph algorithms/system design

## 12. Real-world Connections

- CPU/job scheduling concepts
- discrete-event simulation
- Dijkstra shortest path
- A* search
- top-k
- merge streams
- timer/event queues
- bandwidth/task prioritization

แต่ production scheduler อาจมี fairness/deadline/policy ซับซ้อนกว่า max integer priority มาก

## 13. Priority Queue vs Queue

Queue:
    next = oldest

Priority Queue:
    next = highest/lowest priority

Priority สามารถเปลี่ยน fairness:
low-priority work อาจ starve ถ้ามี high-priority work เข้ามาตลอด

Aging/fair scheduling เป็น policy layer ที่ structure อย่างเดียวไม่แก้

## 14. Complexity Summary

| Operation | Binary Heap Priority Queue |
|---|---:|
| peek max | Θ(1) |
| push | O(log n), Θ(log n) worst |
| pop max | O(log n), Θ(log n) worst |
| size/empty | Θ(1) |
| validate | Θ(n) |
| storage | Θ(capacity) |

Dynamic backing growth adds occasional Θ(n) allocation/copy but geometric growth contributes amortized constant allocation overhead per append layer; heap restoration still dominates push as O(log n).

## Files

- src/max_priority_queue.*
- tests/test_priority_queue.c
- examples/priority_queue_demo.c
- benchmarks/priority_queue_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
