# Chapter 032 — B-Tree

## Goal

B-Tree เป็น multiway balanced search tree ที่ออกแบบให้หนึ่ง node เก็บหลาย keys เพื่อลด tree height และลดจำนวน page/block accesses.

บทนี้สร้าง Integer B-Tree แบบ minimum degree t ตามนิยาม CLRS.

สำหรับ t>=2:
- max keys/node = 2t-1
- max children = 2t
- non-root min keys = t-1
- all leaves อยู่ depth เดียวกัน

รองรับ:
- search
- insert
- split
- delete
- borrow from sibling
- merge
- inorder traversal
- structural validator

## Why B-Tree?

Binary trees fanout=2.

Disk/page storage ต้องการ fanout ใหญ่เพื่อให้:

    height ≈ log_fanout(n)

ต่ำมาก.

หนึ่ง page read สามารถนำ keys/child pointers มาหลายรายการพร้อมกัน.

## Node Invariant

Keys strictly increasing:

    k0 < k1 < ... < k(m-1)

Internal node with m keys has m+1 children.

Ranges:

    child0 < k0
    k0 < child1 < k1
    ...
    k(m-1) < child_m

## Occupancy

For minimum degree t:

non-root:
    t-1 <= key_count <= 2t-1

root:
- may have fewer
- if tree nonempty root has at least 1 key
- root may be leaf

## Search

Within a node:
- find first key >= target
- if equal found
- if leaf not found
- otherwise descend corresponding child

Per-node linear scan in teaching code.
Production pages may use binary search/SIMD.

## Insert

Never descend into a full child.

If root full:
1. allocate new root
2. old root becomes child0
3. split child0
4. descend into appropriate half

split full child y:
- median key moves into parent
- left half remains y
- right half moves to new sibling z

Then insert into non-full leaf.

## Delete

Deletion is harder because we must preserve minimum occupancy.

Before descending into child with only t-1 keys:
make sure it has at least t keys by:
- borrow from left sibling, or
- borrow from right sibling, or
- merge with sibling + separator

When deleting key from internal node:
1. if left child has >=t keys, replace with predecessor then delete predecessor
2. else if right child has >=t keys, replace with successor then delete successor
3. else merge both children with separator then recurse

## Root Shrink

After deletion, root may have zero keys.

If internal:
    promote its only child as new root

If leaf:
    tree becomes empty

This is the only place tree height decreases.

## Correctness

B-Tree preserves:
- sorted keys inside each node
- separator range constraints
- occupancy bounds
- same leaf depth
- unique keys

Validator checks all of these.

## Height

For minimum degree t, minimum branching is roughly t below root.

Height:

    O(log_t n)

Larger page fanout yields very shallow trees.

## External-Memory Model

In real storage engines:
- each B-Tree node maps to a page/block
- comparisons inside a loaded page are cheap relative to I/O
- split/merge may dirty pages
- crash consistency requires WAL/copy-on-write/etc.

This chapter focuses on in-memory mechanics while explaining page-oriented motivation.

## Complexity

Let h be tree height.

Search:
    O(t*h) with linear in-node scan
    often described O(log n) for fixed t

Insert:
    O(t*h)

Delete:
    O(t*h)

Traversal:
    Theta(n)

Space:
    Theta(n)

I/O model:
    O(h) page reads/writes up to constants.

## Files

- src/int_btree.*
- tests/test_btree.c
- examples/btree_demo.c
- benchmarks/fanout_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
