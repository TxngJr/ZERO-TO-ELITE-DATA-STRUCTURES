# Chapter 010 — Queue

## Goal

Queue เป็น ADT แบบ FIFO (First In, First Out): element ที่เข้าเก่าที่สุดออกก่อน บทนี้จะสร้าง Queue สอง representation เพื่อให้เห็นว่าพฤติกรรมเดียวกันทำได้ทั้ง linked nodes และ circular array

หลังจบบทนี้คุณควร:
- นิยาม Queue ADT และ FIFO law
- แยก enqueue/dequeue/front/back
- implement linked queue
- implement circular-array queue
- เข้าใจ head/tail index และ wrap-around
- อธิบายปัญหา "array queue ที่ shift ทุก dequeue"
- วิเคราะห์ complexity ของแต่ละ representation
- เข้าใจ bounded vs dynamically growing queue
- เขียน invariant checker
- ทำ randomized differential tests
- เชื่อม Queue กับ BFS, scheduling, buffering และ producer/consumer concepts

## 1. Queue ADT

Abstract state:

    front [10,20,30] back

Operations:
- enqueue(x): เพิ่มที่ back
- dequeue(): เอาจาก front
- peek/front(): ดูตัวหน้า
- size()
- is_empty()

FIFO law:

ถ้า enqueue a, b, c ตามลำดับ
dequeue ต้องคืน a ก่อน b ก่อน c

## 2. Linked Queue

Representation:

    head                              tail
     |                                  |
     v                                  v
    [10|*] -> [20|*] -> [30|NULL]

enqueue:
- allocate new node
- old tail->next = new
- tail = new

dequeue:
- remove head
- head = head->next
- ถ้าว่างหลังลบ ให้ tail=NULL

เมื่อเก็บทั้ง head และ tail:
- enqueue Θ(1)
- dequeue Θ(1)

## 3. Naive array queue ที่ไม่ควรทำ

ถ้าเก็บ front ที่ index 0 แล้ว dequeue ด้วยการ shift:

    [10][20][30][40]
     remove 10
    [20][30][40][  ]

ต้อง move Θ(n) elements ทุก dequeue

Queue ที่ดีไม่ควร shift ทั้ง array ทุกครั้ง

## 4. Circular Queue

ใช้ array เป็นวงแหวนเชิงตรรกะ

metadata:
- data
- capacity
- head = index ของ front
- size

ตำแหน่ง logical i:

    physical = (head + i) mod capacity

tail insertion position:

    (head + size) mod capacity

ตัวอย่าง capacity=5:

    physical: [30][40][ ][10][20]
                         ^
                         head=3

logical order:
    10,20,30,40

## 5. Wrap-around

เมื่อ index ถึง capacity-1 แล้ว element ถัดไปกลับไป index 0

จุดสำคัญ:
- physical order ไม่จำเป็นต้องตรง logical order
- iteration ต้องใช้ modulo/wrap logic
- resize ต้อง copy logical order กลับมา contiguous ใน buffer ใหม่

## 6. Growing circular queue

เมื่อ full:
1. allocate capacity ใหม่แบบ geometric
2. copy logical elements 0..size-1 ไป new_data[0..size-1]
3. free old data
4. data=new_data
5. head=0

หลัง growth representation canonical อีกครั้ง

## 7. Queue Invariants

CircularQueue:
- size <= capacity
- capacity==0 iff data==NULL ใน initial empty representation นี้
- ถ้า size>0 และ capacity>0, head < capacity
- logical element i อยู่ data[(head+i)%capacity]

LinkedQueue:
- size=0 iff head=tail=NULL
- non-empty tail->next=NULL
- exactly size nodes reachable from head
- last node=tail

## 8. Complexity

| Operation | Circular Array Queue | Linked Queue |
|---|---:|---:|
| front | Θ(1) | Θ(1) |
| enqueue | Θ(1) amortized | Θ(1) |
| dequeue | Θ(1) | Θ(1) |
| size | Θ(1) | Θ(1) |
| grow | Θ(n) occasional | N/A |
| per-element allocation | no | yes |

Circular queue มักได้ locality ดีกว่า
Linked queue ไม่ต้องย้าย backing block ตอนโต แต่ allocate ต่อ node

## 9. Bounded Queue

บางระบบใช้ fixed capacity:
- networking
- embedded systems
- telemetry
- real-time pipelines

เมื่อ full ต้องมี policy:
- reject new item
- drop oldest
- overwrite
- block producer

Policy เป็นส่วนของ Queue contract ไม่ใช่รายละเอียดเล็กน้อย

## 10. BFS connection

Breadth-First Search ใช้ Queue:

    enqueue(start)
    while queue not empty:
        v=dequeue()
        enqueue unvisited neighbors

FIFO ทำให้ vertices ถูก process ตาม "wave" ของระยะทางใน unweighted graph

Graph traversal จะลงลึกใน Chapters 038–040

## 11. Producer / Consumer preview

Queue เป็น boundary ธรรมชาติระหว่าง producer กับ consumer

แต่ Queue ธรรมดาในบทนี้ **ไม่ thread-safe**

Concurrent queue ต้องคิดเรื่อง:
- mutex/condition variable
- atomics
- memory ordering
- lock-free/wait-free properties

จะเรียน Chapters 096–102

## 12. When NOT to use Queue

ถ้าต้อง:
- remove newest → Stack
- remove both ends → Deque
- remove highest priority → Priority Queue
- random access → Array/Vector

## Files

- src/circular_queue.*
- src/linked_queue.*
- tests/test_queues.c
- examples/queue_demo.c
- benchmarks/queue_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
