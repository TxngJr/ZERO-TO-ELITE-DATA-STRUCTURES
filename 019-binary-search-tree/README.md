# Chapter 019 — Binary Search Tree

## Goal

Binary Search Tree (BST) เพิ่ม ordering invariant ลงบน Binary Tree เพื่อให้ search/insert/delete ตัด subtree ที่ไม่เกี่ยวข้องออกได้.

หลังจบบทนี้คุณควร:
- นิยาม strict BST invariant
- search/insert/delete
- minimum/maximum
- successor/predecessor
- เข้าใจ 0/1/2-child deletion
- ใช้ transplant
- validate ordering + parent links
- อธิบาย O(h) operations
- แยก average-ish shape จาก worst-case skew
- พิสูจน์ inorder sorted
- ทดสอบแบบ randomized differential กับ reference set

## Duplicate Policy

Implementation นี้ใช้ unique integer keys:

    left.key < node.key < right.key

Insert key ซ้ำคืน false และ tree ไม่เปลี่ยน.

Duplicate policy ต้องประกาศเสมอ เพราะมีผลต่อ ordering invariant และ deletion/search semantics.

## Search

เริ่ม root:
- key == node => found
- key < node => go left
- key > node => go right

แต่ละ step ลด search space ไปหนึ่ง subtree.

Complexity:
    O(h)

balanced-ish:
    O(log n)

skewed:
    O(n)

## Insert

เดินเหมือน search จนเจอ NULL slot แล้ว attach node ใหม่.

Parent pointer ถูกตั้งทันที.

Insert ไม่ rebalance ใน Chapter นี้.

ดังนั้น sorted insertion:

    1,2,3,4,5

สร้าง right-skewed chain.

## Minimum / Maximum

Minimum:
ตาม left จน NULL.

Maximum:
ตาม right จน NULL.

Cost:
    O(h)

## Successor

ถ้า node มี right subtree:
    successor = minimum(right)

ถ้าไม่มี:
    climb parents จนเจอ ancestor ที่ current อยู่ใน left subtree.

Predecessor กลับทิศ.

## Delete — Case 1: No Children

Detach leaf และ free.

## Delete — Case 2: One Child

เชื่อม parent ของ node ไป child โดยตรง.

## Delete — Case 3: Two Children

ใช้ inorder successor y = minimum(node->right).

ถ้า y ไม่ใช่ direct right child:
- transplant y with y->right
- attach y->right = node->right

จากนั้น:
- transplant node with y
- y->left = node->left

วิธีนี้รักษา ordering invariant.

## Transplant

transplant(u,v) แทน subtree rooted at u ด้วย subtree rooted at v ที่ตำแหน่งเดิมของ u.

มันดูแล:
- root replacement
- parent child link
- v->parent

แต่ไม่ย้าย children อื่นให้อัตโนมัติ.

## Why Inorder Is Sorted

Induction:

For node k:
- every key in left < k
- inorder(left) sorted
- visit k
- every key in right > k
- inorder(right) sorted

ดังนั้น concatenation:
    inorder(left), k, inorder(right)
เป็น ascending sequence.

## Complexity

| Operation | Time |
|---|---:|
| search | O(h) |
| insert | O(h) |
| delete | O(h) |
| min/max | O(h) |
| successor/predecessor | O(h) |
| full inorder | Theta(n) |
| validate | Theta(n) |

Without balancing:
    h can be n-1

## Why BST Can Lose

Hash table expected membership may be O(1), while BST O(h).

BST offers order capabilities:
- min/max
- successor/predecessor
- sorted traversal
- range-style navigation

Balanced BST later gives deterministic logarithmic-height guarantees under its invariants.

## Files

- src/int_bst.*
- tests/test_bst.c
- examples/bst_demo.c
- benchmarks/bst_shape_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
