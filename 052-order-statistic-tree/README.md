# Chapter 052 — Order Statistic Tree

## Goal

Order Statistic Tree คือ balanced BST ที่เพิ่ม metadata:

    subtree_size

ให้ทุก node.

นอกจาก search/insert/remove แล้ว เราจึงตอบคำถามเชิงลำดับได้:

- select(k): key ที่มี rank k
- rank(key): จำนวน keys ที่น้อยกว่า key
- count_range(low,high): จำนวน keys ใน [low,high)

Concrete implementation ใช้ AVL Tree + subtree_size.

## Core Invariant

For every node:

    subtree_size(node)
      = 1
      + subtree_size(left)
      + subtree_size(right)

และยังคง AVL invariant:

    |height(left)-height(right)| <= 1

## select(k)

ให้:

    L = subtree_size(left)

ถ้า:

    k < L
        ไปซ้าย

    k == L
        current key คือคำตอบ

    k > L
        ไปขวาด้วย
        k = k-L-1

เพราะ BST inorder คือ sorted order.

## rank(key)

rank ในบทนี้นิยามเป็น:

    จำนวน keys ที่ strictly less than key

ดังนั้น rank ใช้ได้แม้ key ไม่ได้อยู่ใน tree.

ระหว่าง search:

- key <= node.key -> ไปซ้าย
- key > node.key:
    keys ใน left ทั้งหมด + node เอง
    น้อยกว่า key แน่นอน

สะสม:

    1 + subtree_size(left)

แล้วไปขวา.

## count_range

สำหรับ half-open key range:

    [low, high)

ใช้:

    rank(high) - rank(low)

โดยไม่ต้อง scan keys ในช่วง.

## Why Augmentation Works

AVL rotation เปลี่ยน topology แต่ไม่เปลี่ยน inorder order.

หลัง rotation เราจึง recompute ทั้ง:

    height
    subtree_size

จาก children ใหม่.

นี่คือ pattern สำคัญ:

    balanced tree
    + local metadata
    + local repair after rotation
    = richer query set

Chapter 049 ทำแบบเดียวกันกับ max_high.
บทนี้ทำกับ subtree_size.

## Duplicates

Concrete tree เป็น set:
- duplicate key ถูก reject

ถ้าต้องการ multiset:
- เพิ่ม frequency ต่อ node
- subtree_size ต้องนับ frequency
- rank/select logic ต้องปรับตาม frequency

## Complexity

| Operation | Complexity |
|---|---:|
| contains | O(log n) |
| insert | O(log n) |
| remove | O(log n) |
| rank | O(log n) |
| select | O(log n) |
| count range | O(log n) |
| storage | Theta(n) |

## Files

- src/int_order_stat_tree.*
- tests/test_order_stat_tree.c
- examples/order_stat_demo.c
- benchmarks/rank_select_benchmark.c
- standard chapter learning artifacts
