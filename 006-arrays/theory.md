# Theory — Arrays

## Array object vs pointer object

Given:

    int a[4];

a คือ array object ที่มี 4 int subobjects

Given:

    int *p = a;

p คือ pointer object ที่เก็บ address ของ a[0]

สองสิ่งไม่เหมือนกัน แม้ array expression จะ decay เป็น pointer ในหลายบริบท

ตัวอย่าง:

    sizeof(a)

ใน scope ที่ a เป็น actual array ให้ total byte size ของ array

แต่ใน function parameter:

    void f(int a[])

a ใน parameter declaration ถูกปรับเป็น pointer type; sizeof(a) จึงเป็น pointer size ไม่ใช่ caller array length

## Pointer arithmetic

ถ้า p ชี้ int:

    p + 1

หมายถึง pointer ไป int ถัดไป ไม่ใช่เพิ่ม address integer แค่ 1 byte

compiler scaling ตาม sizeof(int)

pointer arithmetic ต้องอยู่ภายใต้กฎของ array object/one-past boundary

## One-past pointer

C อนุญาตสร้าง pointer หนึ่งตำแหน่งหลัง element สุดท้ายเพื่อ comparison/iteration:

    begin = &a[0]
    end   = &a[n]

แต่ห้าม dereference end

นี่เป็นรากของ half-open range [begin,end)

## Row-major representation

For:

    int m[R][C];

m[r] คือ array C ints
m[r][c] เข้าถึง element ใน row r, column c

row-major ช่วยให้ loop order:

    for r
        for c

เดิน contiguous กว่า column-first ในหลาย workload

## Dynamic array abstraction

Built-in array ไม่ resize ตัวเอง

Dynamic array ใช้ indirection:
- control object
- separately allocated storage
- size/capacity metadata

growth จึงเปลี่ยน backing allocation โดย interface ยังเป็น logical sequence เดิม

## Memory utilization

capacity > size หมายถึง reserved but unused slots

load factor แบบนี้ไม่ใช่ hash-table load factor แต่เป็น utilization:

    size / capacity

geometric growth แลก spare memory เพื่อ amortized performance

## Element type matters

ใน IntVector element เป็น int:
- trivial copy
- no destructor
- memmove ใช้ได้

generic C++ vector ต้องคิดเพิ่ม:
- constructors/destructors
- move/copy exceptions
- alignment
- allocator
- non-trivial object lifetime

Chapter 158 จะกลับมาดู C++ containers ลึกขึ้น
