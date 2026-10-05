# Chapter 011 — Deque

## Goal

Deque (Double-ended Queue) เป็น ADT ที่เพิ่ม/ลบได้ทั้ง front และ back บทนี้จะ implement ring-buffer deque จากศูนย์ และต่อยอดไป Monotonic Queue สำหรับ sliding-window maximum

หลังจบบทนี้คุณควร:
- นิยาม Deque ADT
- implement push_front/push_back/pop_front/pop_back
- เข้าใจ circular-buffer indexing สองทิศทาง
- วิเคราะห์ wrap-around และ growth
- ใช้ Deque เป็น Queue หรือ Stack ได้
- เข้าใจ Monotonic Queue
- แก้ Sliding Window Maximum ใน Θ(n)
- พิสูจน์ aggregate complexity จาก push/pop bounded counts
- เขียน randomized differential tests

## 1. Deque ADT

Operations:
- push_front(x)
- push_back(x)
- pop_front()
- pop_back()
- front()
- back()
- size()
- empty()

Deque ครอบคลุม behavior ของ:
- Queue: push_back + pop_front
- Stack: push_back + pop_back

แต่ API ที่กว้างขึ้นก็เปิดโอกาสให้ misuse มากขึ้น จึงยังมีเหตุผลในการใช้ Queue/Stack ที่แคบกว่าเมื่อ semantics ชัด

## 2. Ring Buffer Representation

Fields:
- data
- capacity
- head
- size

head ชี้ logical front

logical index i:

    physical = wrap(head+i)

back element:

    logical index size-1

push_back:
    write at wrap(head+size)

push_front:
    head = previous(head)
    write data[head]

pop_front:
    read head
    head = next(head)

pop_back:
    read wrap(head+size-1)
    size--

## 3. Previous Index

สำหรับ capacity C:

    prev(0)=C-1
    prev(i)=i-1

นี่ช่วยหลีกเลี่ยง unsigned underflow จาก:

    (head - 1) % capacity

ซึ่งผิดได้ใน C เมื่อใช้ size_t

## 4. Growth

เมื่อ full:
1. allocate larger buffer
2. copy logical sequence front→back ไป index 0..size-1
3. free old
4. head=0

หลัง grow push_front/back ดำเนินต่อได้ตาม invariant เดิม

## 5. Complexity

| Operation | Ring Deque |
|---|---:|
| front/back | Θ(1) |
| push_front/back | Θ(1) amortized |
| pop_front/back | Θ(1) |
| grow | Θ(n) occasional |
| indexed logical access (ถ้ามี) | Θ(1) |

## 6. Monotonic Queue

Monotonic Queue ไม่ใช่ ADT ใหม่ที่จำเป็นต้องมี container พิเศษเสมอไป แต่เป็น pattern ที่ใช้ Deque เก็บ candidate indices โดยรักษาลำดับ monotonic

สำหรับ Sliding Window Maximum:
- deque เก็บ indices
- values ตาม indices ลดลงจาก front→back
- front คือ index ของ maximum ปัจจุบัน

## 7. Sliding Window Maximum

Input:

    [1,3,-1,-3,5,3,6,7]
    k=3

Output:

    [3,3,5,5,6,7]

สำหรับ index i:
1. ลบ indices ด้านหน้า ถ้าออกนอก window
2. ลบ indices ด้านหลังขณะที่ value ใหม่ >= value ที่หลัง เพราะ candidate เก่าจะไม่มีวันชนะใน window อนาคตที่มี value ใหม่อยู่
3. push i
4. เมื่อ window ครบ k, answer = input[deque.front]

## 8. Why Θ(n)?

แต่ละ index:
- push_back หนึ่งครั้ง
- pop_front ได้อย่างมากหนึ่งครั้ง
- pop_back ได้อย่างมากหนึ่งครั้ง

ดังนั้น total deque mutations O(n)

แม้มี while loop ซ้อนใน for แต่ aggregate work ยัง linear

## 9. Duplicate Values

สำหรับ maximum:
ถ้าใช้:

    while new_value >= back_value:
        pop_back

เราเก็บ candidate ใหม่กว่าเมื่อค่าเท่ากัน

ข้อดี:
- index ใหม่หมดอายุช้ากว่า
- deque สั้นลง

ใช้ > แทน >= ก็ยังทำ algorithm ได้ถ้า expiry logic ถูก แต่ invariant/detail ต่างกัน

## 10. Deque vs Queue vs Stack

Deque ยืดหยุ่นกว่า แต่เลือก abstraction ที่แคบที่สุดที่ตรง problem intent ช่วย:
- อ่าน code ง่าย
- ลด invalid operations
- สื่อ semantics ชัด

## 11. Real-world uses

- work queues บางแบบ
- task stealing concepts
- sliding-window algorithms
- undo/redo boundaries บาง design
- buffering
- BFS variants 0-1 BFS ใช้ deque
- schedulers

## 12. 0-1 BFS preview

Graph edges weight 0 หรือ 1:
- weight 0 → push_front
- weight 1 → push_back

ทำให้ shortest-path algorithm ใช้ Deque ได้

จะกลับมาใน graph chapters

## Files

- src/int_deque.*
- src/monotonic_queue.*
- tests/test_deque.c
- examples/deque_demo.c
- benchmarks/deque_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
