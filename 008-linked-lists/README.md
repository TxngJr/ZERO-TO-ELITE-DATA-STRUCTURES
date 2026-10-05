# Chapter 008 — Linked Lists

## Goal

Linked List เปลี่ยนจาก contiguous representation ของ array ไปเป็น nodes ที่เชื่อมกันด้วย pointers บทนี้จึงเป็นจุดที่ pointer ownership, allocation และ invariants เริ่มสำคัญมาก

หลังจบบทนี้คุณควร:
- implement Singly Linked List
- implement Doubly Linked List
- เข้าใจ Circular Linked List
- วาด head/tail/prev/next ownership graph
- วิเคราะห์ prepend/append/find/insert/erase
- เข้าใจ pointer chasing และ locality trade-off
- เขียน invariant checker
- ป้องกัน memory leak/use-after-free/dangling links
- เข้าใจ sentinel node concept
- เลือก list vs dynamic array จาก workload แทนการจำตาราง Big-O อย่างเดียว

## 1. Singly Linked List

Node:

    +-------+------+
    | value | next |
    +-------+------+

List:

    head
      |
      v
    [10|*] -> [20|*] -> [30|NULL]

แต่ละ node สามารถอยู่คนละ allocation

## 2. Core invariant — singly list

สำหรับ implementation ในบท:
- size=0 ⇒ head=NULL และ tail=NULL
- size>0 ⇒ head!=NULL และ tail!=NULL
- tail->next=NULL
- เดิน next จาก head ต้องพบ exactly size nodes
- node สุดท้ายต้องเป็น tail
- ไม่มี cycle

Invariant checker ช่วยจับ pointer bugs ได้เร็วกว่ารอ crash

## 3. Prepend

สร้าง node ใหม่:

    new->next = head
    head = new

ถ้า list เดิมว่าง:
    tail = new

Θ(1)

## 4. Append

ถ้ามี tail pointer:

    tail->next = new
    tail = new

Θ(1)

ถ้าเก็บแค่ head แล้วต้องเดินหาท้าย:
    Θ(n)

Metadata choice จึงเปลี่ยน operation cost

## 5. Search / Index

ต้องเดิน node ทีละตัว:

    head -> next -> next -> ...

get(index i):
    Θ(i), worst Θ(n)

ต่างจาก array ที่คำนวณ address ได้ Θ(1)

## 6. Insert / Erase

ถ้ามี pointer ไป predecessor อยู่แล้ว:
- insert-after Θ(1)
- erase-after Θ(1)

แต่ถ้า API รับ index:
ต้องเดินไปตำแหน่งก่อน → Θ(n)

Big-O ของ operation จึงขึ้นกับสิ่งที่ caller มีอยู่ก่อน

## 7. Doubly Linked List

Node:

    +------+-------+------+
    | prev | value | next |
    +------+-------+------+

    NULL <- [10] <-> [20] <-> [30] -> NULL

ข้อดี:
- เดินย้อนกลับ
- erase node ที่มี pointer อยู่แล้วทำได้โดยไม่ต้องหา predecessor
- pop back Θ(1) ถ้ามี tail

ข้อเสีย:
- pointer เพิ่มต่อ node
- update links ซับซ้อน
- memory footprint และ pointer traffic เพิ่ม

## 8. Doubly-list invariants

- head->prev=NULL
- tail->next=NULL
- สำหรับทุก adjacent pair A,B:
      A->next == B
      B->prev == A
- forward traversal count = size
- backward traversal count = size
- endpoints สอดคล้องกัน

## 9. Circular Linked List

Circular singly:

    head
     |
     v
    [A] -> [B] -> [C]
     ^             |
     +-------------+

ไม่มี NULL sentinel ที่ปลาย
ถ้าเก็บ tail:
    tail->next = head

ใช้กับ:
- round-robin scheduling concepts
- cyclic buffers/players/tasks
- repeated traversal

ต้องมี stopping rule ที่ชัดเจน เช่น iterate exactly size nodes มิฉะนั้น loop ไม่จบ

## 10. Sentinel node

Sentinel เป็น dummy node ที่ลด special cases

แทน:

    NULL <- first ... last -> NULL

อาจใช้ sentinel:

    sentinel <-> first <-> ... <-> last <-> sentinel

ข้อดี:
- insert/erase logic uniform
- fewer branches

ข้อเสีย:
- extra node
- abstraction ต้องชัดว่า sentinel ไม่ใช่ logical element

## 11. Ownership

ใน implementation นี้ list owns every allocated node

Rules:
- create node on insertion
- unlink before free
- free each node exactly once
- destroy walks all nodes

Returning a raw node pointer to caller would complicate lifetime contract; public API จึงใช้ indices/values เพื่อให้ ownership boundary ชัด

## 12. Pointer-to-pointer technique

ใน C singly list บาง operation ใช้:

    Node **link

เพื่ออ้างถึง "ช่อง pointer ที่ต้องเปลี่ยน" ไม่ว่าจะเป็น head หรือ node->next

เทคนิคนี้ลด special case แต่ต้องเข้าใจ pointer-to-pointer ก่อน

บทนี้ใช้ implementation ที่อ่านง่ายเป็นหลัก แล้ว exercises ให้ลอง pointer-to-pointer

## 13. Locality and performance

Array:

    [A][B][C][D]

List:

    [A] ----> [B] ----> [C] ----> [D]
     random-ish allocations possible

แม้ traversal ทั้งคู่ Θ(n), array มักได้เปรียบจาก:
- contiguous cache lines
- prefetch
- fewer allocations
- less metadata

Linked list ได้เปรียบเมื่อ:
- insertion/removal at known nodes dominates
- stable node addresses matter
- splicing/link operations useful

อย่าเลือก list แค่เพราะ "insert O(1)" ถ้าต้องหา index ก่อนทุกครั้ง

## 14. Memory overhead

Singly node:
    value + next + padding/alignment

Doubly:
    prev + value + next + padding

สำหรับ element เล็ก pointer metadata อาจใหญ่กว่า payload มาก

## 15. Circular-list danger

ถ้า destructor ใช้:

    while (node != NULL)

กับ circular list จะไม่ terminate

ต้อง:
- track size
- break when returning to start
- first sever cycle then destroy

representation dictates safe algorithms

## 16. Array vs List summary

| Property | Dynamic Array | Singly List | Doubly List |
|---|---|---|---|
| index access | Θ(1) | Θ(n) | Θ(n) |
| prepend | Θ(n) | Θ(1) | Θ(1) |
| append with tail | amortized Θ(1) | Θ(1) | Θ(1) |
| erase known node | shifting needed | predecessor needed | Θ(1) with node |
| locality | strong | weak | weak |
| per-element metadata | low | pointer | two pointers |
| stable node addresses | no after realloc | yes until node erased | yes until erased |

## Files

- src/int_slist.*
- src/int_dlist.*
- src/int_clist.*
- tests/test_linked_lists.c
- examples/list_demo.c
- benchmarks/list_vs_vector.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
