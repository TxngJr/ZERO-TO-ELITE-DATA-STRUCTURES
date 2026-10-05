# Chapter 034 — B* Tree

## Goal

B* Tree เป็น high-occupancy variant ของ B-Tree. จุดสำคัญไม่ใช่แค่ split node ที่เต็ม แต่คือ:

1. **redistribute กับ sibling ก่อน**
2. ถ้า sibling ก็เต็ม จึง **split 2 nodes เป็น 3 nodes**

แนวคิดนี้เพิ่ม minimum occupancy ของ non-root nodes จากประมาณ 1/2 ใน ordinary B-Tree ไปใกล้ 2/3.

Teaching implementation ใช้ B-tree-style promoted separator keys เพื่อให้เห็น redistribution และ 2-to-3 split ตรง ๆ.

## Order Convention

กำหนด:

    m = maximum children per node
    max_keys = m - 1

บทนี้รับเฉพาะ:

    m >= 3
    m % 3 == 0

เพื่อให้ occupancy arithmetic ของ 2-to-3 split ชัดเจน.

สำหรับ non-root node:

    min_children = 2m/3
    min_keys = 2m/3 - 1

ตัวอย่าง m=6:

    max children = 6
    max keys     = 5
    min children = 4
    min keys     = 3

## Why Higher Occupancy Matters

ถ้า page/node มี capacity เท่าเดิม แต่แต่ละ node เต็มมากขึ้น:
- fanout effective สูงขึ้น
- tree height อาจลด
- disk/page reads อาจลด
- space utilization ดีขึ้น

Trade-off:
- insertion logic ซับซ้อนขึ้น
- redistribution touches sibling + parent
- 2-to-3 split writes more pages/nodes than simple 1-to-2 split

## Insert Path

Search child แบบ ordinary B-Tree.

เมื่อ child overflow มี:

    max_keys + 1

keys.

### Case 1 — Left sibling has room

รวมเชิง logical:

    left keys + parent separator + overflow child keys

แล้ว redistribute ใหม่เป็นสอง nodes และ separator ใหม่หนึ่งตัว.

### Case 2 — Right sibling has room

ทำแบบ mirror.

### Case 3 — Both relevant siblings full

รวมสอง siblings + parent separator + overflow key set แล้วกระจายเป็น:

    node A
    separator 1
    node B
    separator 2
    node C

parent จึงเพิ่มหนึ่ง key และหนึ่ง child.

นี่คือ B* 2-to-3 split.

## Root Overflow

Root ไม่มี sibling/parent ให้ redistribute.

เมื่อ root overflow:
- split root เป็นสอง children
- promote one separator into a new root

Root split เป็น special case และช่วงแรก children ใต้ root อาจต่ำกว่า 2/3 occupancy ได้.

Validator จึงอนุญาต occupancy exception เมื่อ parent คือ root ที่มี separator เดียว.

นี่สอดคล้องกับธรรมชาติของ high-occupancy multiway trees: root มี occupancy rules ผ่อนปรนกว่าปกติ.

## Search

เหมือน B-Tree:

1. binary/linear search ภายใน node
2. ถ้าเจอ key return
3. ถ้า leaf -> miss
4. descend child interval

Teaching codeใช้ linear scan ภายใน nodeเพื่อให้อ่านง่าย.
Production page tree มักใช้ binary search / SIMD / branch-aware search.

## Deletion: Production Theory

Production B* deletion ต้องพยายาม:

1. borrow / redistribute จาก siblings
2. ถ้ายังรักษา 2/3 occupancy ไม่ได้ อาจ redistribute/merge across multiple siblings
3. update parent separators
4. shrink root เมื่อจำเป็น

มันซับซ้อนกว่า B-Tree deletion เพราะ occupancy floor สูงกว่า 1/2.

## Deletion in This Teaching Implementation

เพื่อไม่แอบซ่อน invariant violation:

    remove(key)

ทำแบบ transactional rebuild:

1. inorder keys ทั้งหมด
2. สร้าง B* tree ใหม่ด้วย order เดิม
3. reinsert ทุก key ยกเว้น target
4. ถ้าสำเร็จ swap tree ใหม่เข้ามา
5. ถ้า allocation/insert fail ให้ tree เดิมยังอยู่

Complexity:

    O(n log n)

นี่ **ไม่ใช่ production-optimal B* deletion**.

แต่:
- semantics ถูกต้อง
- B* insertion/occupancy invariant หลัง deletion ยังถูกต้อง
- failure mode ปลอดภัยกว่า half-implemented multi-sibling deletion

Production deletion algorithm ถูกอธิบายใน theory/lab และจะเชื่อมต่อกับ storage-engine chapters ภายหลัง.

## Height

ให้ effective minimum branching factorประมาณ 2m/3.

Height จึงยัง:

    O(log_m n)

และ higher occupancy ช่วยลด constant/height เชิง page count.

## B-Tree vs B* Tree

B-Tree:
- split full child into 2
- non-root minimum occupancy ~1/2
- simpler

B*:
- sibling redistribution before split
- 2 full nodes -> 3 nodes
- minimum occupancy ~2/3
- more mutation work per structural change
- potentially better page utilization

## Page-Oriented Motivation

ถ้า node = disk page:
- empty slots คือ wasted page capacity
- height increase อาจเพิ่ม random I/O
- redistribution may be worthwhile to avoid page split

แต่ real databases ต้องพิจารณา:
- concurrency/latches
- WAL
- crash recovery
- variable-length keys
- prefix compression
- page fragmentation

บทนี้ยังเป็น in-memory integer model.

## Complexity

Let h be tree height.

| Operation | Teaching implementation |
|---|---:|
| contains | O(h * m) |
| insert | O(h * m) plus redistribution/split |
| inorder | Theta(n) |
| remove | O(n log n) rebuild |
| validate | Theta(n) |
| storage | Theta(n) |

For fixed page order m:

    contains/insert = O(log n)

## Files

- src/int_bstar_tree.*
- tests/test_bstar_tree.c
- examples/bstar_demo.c
- benchmarks/occupancy_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
