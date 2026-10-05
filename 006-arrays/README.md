# Chapter 006 — Arrays

## Goal

Array คือ Data Structure พื้นฐานที่สุดตัวหนึ่งและเป็นฐานของ vector, heap, hash table buckets, matrix/tensor storage และระบบอีกจำนวนมาก บทนี้ลงลึกตั้งแต่ static array ไปถึง dynamic array ที่ implement เอง

หลังจบบทนี้คุณควร:

- อธิบาย contiguous representation ของ array
- คำนวณ conceptual address ของ element จาก base + index × element_size
- แยก length, size และ capacity
- เข้าใจ static array, fixed-capacity array และ dynamic array
- เข้าใจ multidimensional array และ row-major layout ใน C
- เข้าใจ array-to-pointer decay และกรณีที่ sizeof ยังเห็น array จริง
- implement growable IntVector จากศูนย์
- วิเคราะห์ get/set/push/insert/erase/reserve
- พิสูจน์ size <= capacity invariant
- อธิบาย Θ(1) random access และ Θ(n) shifting
- อธิบาย Θ(1) amortized append ภายใต้ geometric growth
- เข้าใจ pointer/reference invalidation หลัง reallocation
- เขียน randomized differential test เทียบกับ reference sequence
- เชื่อม array layout เข้ากับ cache locality จาก Chapter 002

## 1. Static array

C:

    int a[5] = {10,20,30,40,50};

conceptual layout:

    +----+----+----+----+----+
    | 10 | 20 | 30 | 40 | 50 |
    +----+----+----+----+----+
      0    1    2    3    4

elements เป็น objects ชนิดเดียวกันเรียงต่อเนื่องใน array object

ถ้า element size = s และ base คือ address ของ element 0:

    element i อยู่ที่ base + i*s

compiler ทำ scaling ให้ pointer arithmetic ตาม pointed-to type

## 2. Random access

ถ้ารู้ index i เราไม่ต้องเดิน element 0..i-1

address calculation ใช้ bounded arithmetic ภายใต้ RAM-style model จึงได้ Θ(1) access

นี่เป็นข้อได้เปรียบสำคัญเหนือ linked list

## 3. Bounds

valid indices ของ array length n:

    0 <= i < n

ใน C ไม่มี automatic bounds check สำหรับ built-in array indexing

access นอกขอบเขตเป็น undefined behavior

Data Structure API ที่ปลอดภัยกว่าจึงอาจใช้:
- bool + out parameter
- error code
- exception
- Option/Result
- runtime panic ตามภาษา

## 4. Static length vs dynamic logical size

built-in array:

    int a[100];

มี storage 100 ints

แต่ container อาจใช้เพียง 37 items

dynamic vector จึงแยก:

    size     = จำนวน logical elements
    capacity = จำนวน elements ที่ allocated storage รองรับก่อน grow

Invariant:

    0 <= size <= capacity

## 5. Dynamic array

representation ของ IntVector ในบทนี้:

    +----------------------+
    | data ----------------+----> [e0][e1][e2][ ][ ][ ][ ][ ]
    | size = 3             |
    | capacity = 8         |
    +----------------------+

append เมื่อ size < capacity:
1. เขียน data[size]
2. size++

append เมื่อเต็ม:
1. เลือก capacity ใหม่
2. realloc/grow
3. ถ้าสำเร็จ commit pointer
4. เขียน element
5. size++

## 6. Growth policy

ถ้า grow +1 ทุกครั้ง:
sequence ของ appends อาจ copy:

    1 + 2 + 3 + ... + n
    = Θ(n²)

ถ้า grow แบบ geometric เช่น ×2:
copy:

    1 + 2 + 4 + 8 + ...
    = Θ(n)

สำหรับ n appends

จึงได้ Θ(1) amortized append

นี่คือเหตุผลเชิง algorithm ไม่ใช่แค่ trick ของ implementation

## 7. Reserve

reserve(k) ขอให้ capacity >= k โดยไม่เปลี่ยน logical size

มีประโยชน์เมื่อรู้จำนวน elements โดยประมาณ:
- ลดจำนวน allocations
- ลด copies
- ลด pointer invalidation

แต่ reserve มากเกินไปเพิ่ม unused memory

## 8. Insert

insert ที่ index i ต้องเลื่อน suffix:

    before:
    [10][20][30][40][_]

    insert 99 at index 1

    shift right:
    [10][20][20][30][40]

    write:
    [10][99][20][30][40]

จำนวน elements ที่เลื่อน = size - i

front insertion จึง Θ(n) worst case

## 9. Erase

erase index i ต้องเลื่อน suffix ซ้าย:

    [10][20][30][40]
         erase 20
    [10][30][40][?]

logical size ลดลง แต่ capacity ใน implementation นี้ไม่ลดอัตโนมัติ

เหตุผล:
- หลีกเลี่ยง resize thrashing
- reuse storage สำหรับ future pushes

shrink_to_fit เป็น explicit operation

## 10. Multidimensional arrays

C declaration:

    int matrix[ROWS][COLS];

เป็น array of ROWS elements โดยแต่ละ element เป็น array of COLS ints

C ใช้ row-major layout:
row 0 ต่อเนื่องก่อน row 1

conceptual linear index:

    index = row * COLS + column

Chapter 089 จะขยายไป tensor strides และ row-major/column-major อย่างเต็มรูปแบบ

## 11. Array-to-pointer decay

ใน expression หลายบริบท array expression ถูก converted เป็น pointer ไป element แรก

ดังนั้น function:

    void f(int a[])

parameter type มี behavior เทียบเท่า pointer parameter ใน declaration context นั้น

function จึงไม่รู้ built-in array length จาก sizeof(a)

ข้อยกเว้นสำคัญมีหลายบริบท เช่น operand ของ sizeof บน array object จริง

อย่าจำสั้น ๆ ว่า "array คือ pointer" เพราะผิด model:
array object และ pointer object เป็นคนละชนิดของ entity

## 12. Pointer invalidation

ถ้า vector realloc ย้าย block:

    old data pointer ---> old block  X
    vector.data -------> new block

pointer/reference/iterator ที่ชี้ storage เก่าใช้ต่อไม่ได้

แม้ realloc จะไม่ย้ายในบางครั้ง API user ห้ามอาศัยโชคถ้า contract บอกว่า operation อาจ invalidate

## 13. Locality

Sequential array traversal:
- contiguous
- hardware prefetch friendly
- cache line utilization ดีโดยทั่วไป

Random access:
- Θ(1) asymptotically
- แต่ cache miss latency ยังมีผล

Complexity และ locality จึงต้องใช้ร่วมกัน

## 14. Static vs dynamic trade-off

Static/fixed:
- simple
- no resize allocation
- predictable storage
- maximum size ต้องรู้/จำกัด

Dynamic:
- flexible size
- extra capacity
- resize cost
- ownership/allocation complexity
- pointer invalidation

## 15. Cross-language mapping

C:
    built-in arrays + manual dynamic buffers

C++:
    std::array / std::vector

Python:
    list เป็น dynamic array-like sequence implementation conceptually ไม่ใช่ C int array

Rust:
    [T; N] / Vec<T>

Java:
    T[] / ArrayList<T>

Go:
    array / slice; slice เป็น descriptor ที่อ้าง backing array

แต่ละภาษาให้ memory/ownership semantics ต่างกัน

## 16. Complexity table

| Operation | Time | Notes |
|---|---:|---|
| get/set by index | Θ(1) | valid index |
| push without grow | Θ(1) | one write |
| push with grow | Θ(n) | may copy/move elements |
| push amortized with doubling | Θ(1) | over sequence |
| pop back | Θ(1) | no shrinking in this implementation |
| insert front | Θ(n) | shift suffix |
| insert at i | Θ(n-i) | plus possible grow |
| erase at i | Θ(n-i) | shift suffix |
| reserve(k) | up to Θ(n) | if allocation moves/copies |
| clear | Θ(1) here | elements are ints; only size reset |
| shrink_to_fit | up to Θ(n) | may reallocate |

Auxiliary state of the vector object itself is Θ(1), while owned element storage is Θ(capacity).

## 17. When NOT to use dynamic array

ไม่เหมาะเมื่อ:
- insert/delete กลางลำดับบ่อยมากและ shifting เป็น bottleneck
- stable addresses ของ elements ต้องรับประกันตลอด mutation
- data ใหญ่มากจน contiguous allocation เป็นปัญหา
- workload ต้องการ ordering/search property ที่ array เองไม่มี

แต่ห้ามเลือก linked list โดยอัตโนมัติ ต้อง benchmark workload จริงและพิจารณา locality

## Files

- src/int_vector.h/.c — dynamic array implementation
- tests/test_int_vector.c — deterministic randomized differential tests
- examples/array_layout.c — static/multidimensional arrays
- benchmarks/vector_operations.c — push vs front-insert scaling
- theory/visual/complexity/invariants/pitfalls
- lab/exercises/quiz/references
