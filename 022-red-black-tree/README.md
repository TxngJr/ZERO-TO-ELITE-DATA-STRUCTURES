# Chapter 022 — Red-Black Tree

## Goal

Red-Black Tree เป็น self-balancing BST ที่ควบคุม height ผ่าน color invariants แทนการเก็บ exact subtree height แบบ AVL.

หลังจบบทนี้คุณควร:
- อธิบาย Red/Black color invariants
- เข้าใจ black-height
- implement insert fix-up
- implement delete fix-up
- เข้าใจ recoloring + rotations
- พิสูจน์ logarithmic height bound ในระดับแนวคิด
- compare AVL vs Red-Black trade-offs
- validate BST ordering + parent links + colors + black-height
- randomized differential test กับ reference set

## Invariants

สำหรับ implementation นี้ NULL child ถูกมองเป็น black leaf เชิงตรรกะ.

Red-Black properties:

1. ทุก node เป็น RED หรือ BLACK
2. root เป็น BLACK
3. NULL leaves เป็น BLACK
4. RED node ห้ามมี RED child
5. ทุก path จาก node ไป descendant NULL leaves มีจำนวน BLACK nodes เท่ากัน

Property 5 เรียกว่า black-height consistency.

## Why Height Stays Logarithmic

เพราะ RED nodes ห้ามติดกัน:
เส้นทางจาก root ถึง leaf ยาวที่สุดมีจำนวน RED แทรกได้ไม่เกินจำนวน BLACK โดยประมาณ.

อีกด้านหนึ่ง subtree ที่มี black-height b ต้องมีจำนวน internal nodes อย่างน้อยประมาณ:

    2^b - 1

จึงสรุปได้ว่า tree height ถูก bound ด้วยค่าระดับ:

    h <= 2 log2(n+1)

ดังนั้น search/insert/delete:

    O(log n)

## Insert

เริ่มเหมือน BST:
- insert node เป็น RED
- ถ้า parent BLACK ไม่มี violation
- ถ้า parent RED ต้อง fix

เหตุผลที่ insert ใหม่เป็น RED:
การเพิ่ม RED node ไม่เปลี่ยน black-height ของ paths ทันที.

## Insert Fix-up Cases

ให้:
- z = current node
- p = parent
- g = grandparent
- u = uncle

ถ้า p RED:

### Uncle RED
- p -> BLACK
- u -> BLACK
- g -> RED
- continue from g

### Uncle BLACK + triangle
เช่น LR:
- rotate parent ก่อน
- เปลี่ยนเป็น line case

### Uncle BLACK + line
- parent BLACK
- grandparent RED
- rotate grandparent

ท้ายสุดบังคับ root BLACK.

## Delete

Delete เริ่มเหมือน BST deletion.

ถ้า node สี RED ถูกเอาออก:
black-height ไม่เปลี่ยน.

ถ้า BLACK ถูกเอาออก:
path หนึ่งอาจขาด BLACK หนึ่งหน่วย จึงต้อง delete fix-up.

## Delete Fix-up Mental Model

ตัวแปร x แทน subtree ที่มี "extra black" เชิงแนวคิด.

ดู sibling w และสีของ children ของ w.

Cases หลัก:
1. sibling RED
2. sibling BLACK + children BLACK
3. sibling BLACK + near child RED / far child BLACK
4. sibling BLACK + far child RED

ใช้ recolor + rotation เพื่อ:
- ย้าย extra-black upward หรือ
- absorb/resolve extra-black

Mirror cases ใช้ซ้าย/ขวาสลับกัน.

## NULL Without Sentinel Node

CLRS มักใช้ NIL sentinel object.

Teaching implementation นี้ใช้ C NULL จริง แต่:

    color(NULL) = BLACK

ตอน delete fix-up จึงต้องเก็บ parent ของ x แยกเมื่อ x=NULL.

นี่ทำให้เห็นชัดว่าทำไม sentinel ช่วยลด special cases.

## AVL vs Red-Black

AVL:
- tighter height
- stores height/balance metadata
- search paths มักสั้นกว่าเล็กน้อย
- update policy strict

Red-Black:
- looser balance
- color metadata เล็ก
- logarithmic height bound
- insertion/deletion fix-up ใช้ recoloring + rotations

เลือกตาม workload/implementation context ไม่ใช่ชื่อเสียง.

## Complexity

| Operation | Time |
|---|---:|
| contains | O(log n) |
| insert | O(log n) |
| delete | O(log n) |
| inorder | Theta(n) |
| validate | Theta(n) |

Rotations:
    Theta(1)

Storage:
    Theta(n)

## Files

- src/int_red_black_tree.*
- tests/test_red_black_tree.c
- examples/red_black_demo.c
- benchmarks/red_black_vs_avl.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
