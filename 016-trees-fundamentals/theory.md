# Theory — Trees Fundamentals

## Unique-parent invariant

- root ไม่มี parent
- non-root ทุก node มี parent เดียว
- ตาม parent ซ้ำต้องจบที่ root
- cycle และ multiple-parent ถูกห้ามใน ordinary tree model

## Edge-count theorem

Finite non-empty tree ที่มี n nodes มี:

    n-1 edges

เพราะ non-root n-1 nodes แต่ละตัวมี parent edge หนึ่งเส้น.

## Perfect binary tree

At depth d:
    2^d nodes

Through height h:
    2^(h+1)-1 nodes

ความสัมพันธ์ exponential นี้ทำให้ complete/perfect-like tree มี height logarithmic in n.

## Minimum height for n nodes

หา h เล็กสุดที่:

    n <= 2^(h+1)-1

โดยประมาณ:

    h >= ceil(log2(n+1))-1

## Maximum height

For n>0:

    h_max=n-1

เกิดใน chain/skewed tree.

## Structural induction

Base:
พิสูจน์ property สำหรับ Empty.

Step:
สมมติ property จริงสำหรับ left/right subtrees แล้วพิสูจน์สำหรับ Node(left,right).
