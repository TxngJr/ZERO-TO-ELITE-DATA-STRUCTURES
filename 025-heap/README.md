# Chapter 025 — Heap

## Goal

Chapter 012 ใช้ Binary Heap เพื่อสร้าง Priority Queue. Chapter 025 กลับมามอง Heap เป็น data structure โดยตรง และลงลึกเรื่อง shape proof, bottom-up construction, heapify, heapsort, arbitrary replacement, index arithmetic และ cache locality.

บทนี้ใช้ Integer Min-Heap เพื่อไม่ทำซ้ำ Max Priority Queue เดิม.

หลังจบบทนี้คุณควร:
- แยก Heap representation จาก Priority Queue ADT
- เข้าใจ complete-tree array layout
- derive parent/child formulas
- implement sift-up / sift-down
- implement push / peek-min / pop-min
- implement bottom-up build-heap
- พิสูจน์ BUILD-HEAP Theta(n)
- implement heap sort
- เข้าใจ heap is partially ordered, not globally sorted
- validate heap invariant
- compare incremental construction vs bottom-up heapify

## Binary Heap Representation

Complete binary tree stored densely in array:

    parent = (i-1)/2       if i>0
    left   = 2i+1
    right  = 2i+2

เพราะ complete shape จึงไม่มี structural holes ใน indices 0..n-1.

## Min-Heap Invariant

สำหรับ child c และ parent p:

    heap[p] <= heap[c]

ดังนั้น root เป็น global minimum.

Heap array ไม่ได้ sorted ทั้ง array.

## Sift Up

หลัง append:
- compare parent
- ถ้า child เล็กกว่า parent ให้ swap
- repeat upward

Worst:
    O(log n)

## Sift Down

หลัง root replacement:
- เลือก child ที่เล็กที่สุด
- ถ้า current <= child นั้น stop
- ไม่งั้น swap ลง

Worst:
    O(log n)

## Push / Pop

Push:
1. ensure capacity
2. append
3. sift up

Pop:
1. save root
2. move last to root
3. shrink
4. sift down

peek-min:
    Theta(1)

## Bottom-Up BUILD-HEAP

Leaves already satisfy heap order.

Start at last internal node and sift down backward:

    for i = n/2; i > 0; --i:
        sift_down(i-1)

## Why BUILD-HEAP Is Theta(n)

Most nodes are near leaves and can move only a few levels.

Approximate count:
- n/2 nodes height 0
- n/4 height <=1
- n/8 height <=2
- ...

Total work is bounded by:

    n * sum(h / 2^(h+1))

The series converges, so total O(n).

Input itself has n elements, giving Omega(n).

Therefore:

    BUILD-HEAP = Theta(n)

## Incremental vs Bottom-Up

Repeated push:
    O(n log n)

Bottom-up:
    Theta(n)

For bulk input bottom-up is asymptotically better.

## Heap Sort

Ascending in-place:
1. build max-heap
2. swap max with last active slot
3. shrink active heap
4. sift down
5. repeat

Total:
    Theta(n log n)

Auxiliary:
    O(1)

Ordinary heap sort is not stable.

## Heap vs Sorted Array / BST

Heap is excellent for repeated best-element extraction.

Balanced BST is better when needing:
- arbitrary key lookup
- predecessor/successor
- ordered traversal
- range operations

Heap arbitrary-value search is generally Theta(n) without external indexing.

## Indexed Heap Preview

Decrease-key by item identity often uses:
- heap array
- map item -> heap index

Every heap swap must update the map.
This creates a cross-structure invariant.

## Arbitrary Replacement

If replacement becomes smaller:
    sift up

If larger:
    sift down

## Locality

Array heap is compact and pointer-free.
It usually has better memory density than pointer trees.

Large heaps still cross cache levels and sift paths jump farther apart by level.

## Integer Overflow

Expressions like 2*i+1 can overflow size_t for extreme i.

Implementation only forms child indices after proving the node has a live child under current size bounds.

## Complexity Summary

| Operation | Min Heap |
|---|---:|
| peek-min | Theta(1) |
| push | O(log n) |
| pop-min | O(log n) |
| build-heap | Theta(n) |
| validate | Theta(n) |
| arbitrary find | Theta(n) |
| heap sort | Theta(n log n) |
| storage | Theta(n) |
