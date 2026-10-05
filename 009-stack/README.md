# Chapter 009 — Stack

## Goal

Stack เป็น ADT แบบ LIFO (Last In, First Out) บทนี้กลับมาที่ Stack หลังจาก Chapter 003 เคยใช้เป็นตัวอย่าง ADT แต่ตอนนี้เรามีความรู้เรื่อง arrays, dynamic growth, linked lists, complexity และ memory มากพอที่จะเปรียบเทียบ implementations อย่างจริงจัง รวมถึง Monotonic Stack ซึ่งเป็น pattern สำคัญมาก

หลังจบบทนี้คุณควร:
- นิยาม Stack ADT โดยไม่ผูกกับ representation
- implement array-backed stack
- implement linked stack
- เปรียบเทียบ complexity และ memory behavior
- แยก Stack ADT ออกจาก runtime call stack
- เข้าใจ push/pop/peek invariants
- รู้จัก bounded/fixed stack
- เข้าใจ stack underflow
- ใช้ stack แปลง recursion บางรูปเป็น iteration
- เข้าใจ monotonic increasing/decreasing stack
- แก้ Next Greater Element ด้วย monotonic stack
- วิเคราะห์ทำไม monotonic stack algorithm เป็น Θ(n) แม้มี while ซ้อนใน for
- เขียน randomized differential tests ระหว่างสอง implementations

## 1. Stack ADT

Abstract sequence:

    bottom [10, 20, 30] top

Operations:
- push(x)
- pop()
- peek/top()
- is_empty()
- size()

LIFO law เชิงพฤติกรรม:

    pop(push(S,x))

คืน x และเหลือ state S

Stack interface ไม่กำหนดว่าใช้ array หรือ linked nodes

## 2. Array-backed Stack

Representation:

    data: [10][20][30][_][_]
    size: 3
    capacity: 5

top อยู่ data[size-1]

push:
1. ensure capacity
2. data[size]=x
3. size++

pop:
1. --size
2. return data[size]

ข้อดี:
- locality ดี
- metadata ต่อ element ต่ำ
- ไม่ allocate ทุก push

ข้อเสีย:
- grow อาจ relocate
- spare capacity
- raw pointers to elements อาจ invalid หลัง grow

## 3. Linked Stack

    top
     |
     v
    [30|*] -> [20|*] -> [10|NULL]

push:
- allocate node
- node->next=top
- top=node

pop:
- victim=top
- top=top->next
- free victim

ข้อดี:
- push/pop relink Θ(1)
- ไม่มี geometric capacity
- existing node addresses stable จน node ถูกลบ

ข้อเสีย:
- allocation per push ใน implementation ธรรมดา
- pointer metadata
- locality แย่กว่า contiguous array โดยทั่วไป

## 4. Complexity

| Operation | Array stack | Linked stack |
|---|---:|---:|
| peek | Θ(1) | Θ(1) |
| push | Θ(1) amortized, Θ(n) grow worst | Θ(1) at list-operation level |
| pop | Θ(1) | Θ(1) |
| size | Θ(1) | Θ(1) |
| memory | Θ(capacity) | Θ(n) nodes |

Asymptotic table ไม่พอ ต้องดู allocation/cache behavior ด้วย

## 5. Underflow

pop/peek บน empty stack ต้องมี documented behavior

implementation นี้คืน false

## 6. Fixed-capacity stack

บาง embedded/real-time workload กำหนด capacity ล่วงหน้า

ข้อดี:
- no dynamic allocation หลัง startup
- predictable memory

ข้อเสีย:
- hard bound
- push อาจ fail เมื่อเต็ม

## 7. Stack ADT vs Call Stack

Stack ADT:
- data structure ที่โปรแกรมสร้างเอง
- เก็บ values/tasks ตาม API

Call stack:
- execution context ของ active function calls

ทั้งคู่ใช้ LIFO concept แต่ไม่ใช่ object เดียวกัน

Recursive DFS อาจใช้ call stack implicit
Iterative DFS ใช้ explicit Stack ADT

## 8. Monotonic Stack

Monotonic stack คือการใช้ ordinary stack พร้อม order invariant

ตัวอย่าง decreasing stack จาก bottom → top:

    values[s0] >= values[s1] >= ... >= values[top]

ใช้กับ:
- Next Greater Element
- Previous Greater/Smaller
- stock span
- histogram
- waiting-days style problems

## 9. Next Greater Element

Input:

    [2, 1, 5, 3, 4]

Result:

    2 -> 5
    1 -> 5
    5 -> none
    3 -> 4
    4 -> none

Algorithm เก็บ indices ที่ยัง unresolved

เมื่อ current > value at top:
- pop index
- answer[index]=current

จากนั้น push current index

## 10. Why Θ(n), not Θ(n²)?

แม้ code มี while ภายใน for แต่ index แต่ละตัว:
- pushed exactly once
- popped at most once

total pushes <= n
total pops <= n

ดังนั้น total stack operations Θ(n)

นี่เป็น aggregate analysis จาก Chapter 004

## 11. Duplicate semantics

ต้องเลือก contract:
- strictly greater: pop เมื่อ current > top
- greater-or-equal: pop เมื่อ current >= top

operator เปลี่ยนความหมายของโจทย์

## 12. When NOT to use Stack

ถ้าต้อง:
- remove oldest → Queue
- random access → Array/Vector
- remove highest priority → Priority Queue
- membership lookup → Set/Map

## 13. Real-world connections

- parser/compiler states
- explicit DFS
- backtracking
- undo/history concepts
- VM operand stacks
- monotonic-stack algorithm patterns

## Complexity summary

Array push geometric growth:
- worst Θ(n)
- amortized Θ(1)

Linked push/pop:
- Θ(1) link manipulation

Next Greater:
- Θ(n) time
- Θ(n) auxiliary space worst case

## Files

- src/array_stack.*
- src/linked_stack.*
- src/monotonic_stack.*
- tests/test_stacks.c
- examples/stack_demo.c
- benchmarks/stack_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
