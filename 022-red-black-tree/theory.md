# Theory — Red-Black Tree

## Black-height

black-height ของ node คือจำนวน BLACK nodes บน path จาก node ไป descendant NULL leaf ภายใต้ convention ที่กำหนด.

สิ่งสำคัญคือทุก path จาก node เดียวกันต้องได้ค่าเท่ากัน.

## Height Bound Sketch

Collapse RED nodes เข้ากับ BLACK parents.
ผลลัพธ์เชิงแนวคิดมี branching ที่ยังสะท้อน black-height.

subtree black-height b ต้องมีอย่างน้อย:

    2^b - 1

internal nodes.

เพราะ RED ห้ามต่อ RED:
ordinary height h ไม่เกินประมาณ 2b.

จึงได้ logarithmic height.

## Insert vs Delete

Insert violation หลัก:
    red-red

Delete violation หลัก:
    black-height deficit

นี่คือเหตุผลที่ delete fix-up ซับซ้อนกว่า insert.

## Recoloring vs Rotation

Recoloring เปลี่ยน bookkeeping ของ invariants โดยไม่เปลี่ยน shape.
Rotation เปลี่ยน local shape แต่รักษา BST inorder order.

Red-Black fix-up ใช้ทั้งสองอย่างร่วมกัน.
