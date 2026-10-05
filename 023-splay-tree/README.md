# Chapter 023 — Splay Tree

## Goal

Splay Tree เป็น self-adjusting BST ที่ไม่มี color/height metadata. ทุก access จะใช้ rotations ดึง node ที่เข้าถึงขึ้นใกล้ root หรือถึง root.

จุดเด่น:
- operation เดี่ยว worst-case O(n)
- sequence ของ operations มี amortized O(log n)
- keys ที่ถูกใช้งานซ้ำมีแนวโน้มอยู่ใกล้ root

หลังจบบทนี้คุณควร:
- เข้าใจ self-adjusting tree
- implement Zig
- implement Zig-Zig
- implement Zig-Zag
- splay หลัง search/insert
- delete ด้วย split/join idea
- แยก worst-case กับ amortized complexity
- เข้าใจ working-set/locality intuition
- validate BST + parent links
- randomized differential test

## No Explicit Balance Metadata

ต่างจาก AVL/Red-Black:
Splay Tree ไม่เก็บ height/color เพื่อบังคับ balance ทุกเวลา.

Tree สามารถดู skewed ได้ ณ moment หนึ่ง.

Guarantee หลักเป็น amortized over operation sequence.

## Splay Operation

ให้ x เป็น accessed node.

ทำ rotations ซ้ำจน x เป็น root.

### Zig

x มี parent แต่ไม่มี grandparent.

ถ้า x เป็น left child:
    rotate right(parent)

ถ้า right:
    rotate left(parent)

### Zig-Zig

x และ parent อยู่ด้านเดียวกันของ grandparent.

Left-left:
    rotate right(grandparent)
    rotate right(parent)

Right-right mirror.

### Zig-Zag

x กับ parent อยู่คนละทิศ.

Left-right:
    rotate left(parent)
    rotate right(grandparent)

Right-left mirror.

## Search

Search เดินแบบ BST.

ถ้าพบ:
    splay(found)

ถ้าไม่พบ:
    implementation นี้ splay node สุดท้ายที่ตรวจ

การ splay miss สามารถช่วยให้ boundary ใกล้ searched key มาอยู่ใกล้ root.

เพราะ search เปลี่ยน shape API จึงไม่เป็น const operation ในเชิง representation.

## Insert

1. BST insert.
2. ถ้า duplicate:
   - splay existing node
   - return false
3. ถ้าใหม่:
   - attach leaf
   - splay new node to root

ดังนั้น successful insertion ทำให้ key ใหม่เป็น root.

## Delete

1. access/splay target.
2. ถ้าไม่พบ return false.
3. target อยู่ root.
4. detach left and right subtrees.
5. ถ้า left ไม่มี:
       root = right
6. ถ้ามี left:
       root = left
       find maximum(left)
       splay maximum to root
       root.right = original right

เหตุผล:
maximum ของ left subtree ไม่มี right child.
หลัง splay มันจึงเป็นจุด join ที่เหมาะสมกับ right subtree ทั้งก้อน.

## Amortized Complexity

Single operation:
    O(n) worst-case

Amortized over m operations:
    O(log n) per operation ภายใต้ standard splay analysis.

แนวคิด potential function มอง tree shape เป็น stored "potential".
operation ที่แพงมักปรับ shape ให้ operations ถัดไปได้รับประโยชน์.

บท Amortized Structures 131 จะกลับมาพิสูจน์เชิง formal มากขึ้น.

## Working-Set Intuition

ถ้า key เดิมถูกใช้บ่อย:
- access แรก splay มันขึ้น root
- access ซ้ำทันทีกลายเป็น O(1)

Splay Tree จึงเหมาะกับ workloads ที่มี temporal locality บางชนิด.

แต่ไม่มี guarantee ว่า workload จริงทุกแบบจะชนะ AVL/Red-Black.

## Static Optimality / Locality Preview

Splay Trees มีผลทางทฤษฎีที่น่าสนใจ:
- working-set theorem
- static optimality
- dynamic finger properties บางรูปแบบ

รายละเอียดลึกเป็น research-level และจะ revisit Chapter 168.

## Complexity

| Operation | Worst single op | Amortized |
|---|---:|---:|
| search/access | O(n) | O(log n) |
| insert | O(n) | O(log n) |
| delete | O(n) | O(log n) |

Traversal:
    Theta(n)

Validator:
    Theta(n)

## API Semantics

contains/access สามารถเปลี่ยน root แม้ logical set ไม่เปลี่ยน.

นี่แสดงความต่างระหว่าง:
- logical constness
- representation mutation

## Files

- src/int_splay_tree.*
- tests/test_splay_tree.c
- examples/splay_demo.c
- benchmarks/splay_hot_access.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
