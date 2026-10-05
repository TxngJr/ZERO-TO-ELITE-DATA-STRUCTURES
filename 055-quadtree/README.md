# Chapter 055 — Quadtree

## Goal

Quadtree แบ่งพื้นที่ 2D ออกเป็น 4 quadrants ซ้ำ ๆ.

บทนี้ implement **dynamic point-region Quadtree** สำหรับจุด:

    (x, y, id)

รองรับ:
- insert
- rectangle count
- rectangle report
- node/point statistics
- structural validation

Root domain เป็น half-open rectangle:

    [x_low, x_high) × [y_low, y_high)

## Bucketed Leaves

Leaf เก็บ points ได้ถึง:

    bucket_capacity

เมื่อเต็ม:
- ถ้ายัง split ได้และ depth < max_depth -> split เป็น 4 children
- redistribute points
- insert point ใหม่ลง child
- ถ้า split ต่อไม่ได้ -> leaf ขยาย bucket ได้

เหตุผล:
- duplicate coordinates อาจตก quadrant เดิมตลอด
- integer coordinate cell อาจเล็กจน midpoint ไม่สร้างช่วงย่อยที่ไม่ว่าง
- max_depth ป้องกัน pathological subdivision

## Quadrants

Midpoints:

    mid_x
    mid_y

Quadrants:

    0 = southwest
    1 = southeast
    2 = northwest
    3 = northeast

Routing ใช้:

    east  = x >= mid_x
    north = y >= mid_y

## Safe Integer Midpoint

พิกัดเป็น int64_t.

Implementation ใช้ midpoint helper ที่หลีกเลี่ยง overflow กรณี bounds อยู่คนละข้างศูนย์ แทนการคำนวณ:

    (low + high) / 2

ซึ่ง overflow ได้.

## Query Pruning

แต่ละ node รู้ bounds ของตัวเอง.

Rectangle query:

1. node bounds ไม่ intersect query
   -> prune ทั้ง subtree

2. node bounds อยู่ใน query ทั้งหมด
   -> count ใช้ cached subtree_size ได้ทันที

3. partial overlap
   -> ตรวจ bucket points หรือ recurse children

## Internal Node Invariant

Internal node:
- ไม่มี bucket points
- มี children 4 ตัว
- children partition parent bounds แบบไม่ overlap และ cover parent
- subtree_size = sum child subtree_size

Leaf:
- ไม่มี children
- subtree_size = point_count
- ทุก point อยู่ใน leaf bounds

## Complexity

ขึ้นกับ distribution และ depth.

สำหรับ spatially distributed data ที่ split ดี:
- insert โดยประมาณ O(depth)
- range query prune ได้มาก

Worst cases:
- many coincident points
- extremely skewed distribution
- query ครอบพื้นที่เกือบทั้งหมด

สามารถเข้าใกล้ O(n).

Storage:

    O(points + allocated nodes)

## Quadtree vs KD-Tree

Quadtree:
- partition space by fixed geometric quadrants
- empty regions may have no allocated child contents
- natural for maps, occupancy, collision broad phase

KD-Tree:
- split by data-dependent median
- typically balanced by point count in static construction
- useful nearest-neighbor search

## Files

- src/int_quadtree.*
- tests/test_quadtree.c
- examples/quadtree_demo.c
- benchmarks/quadtree_benchmark.c
- standard chapter artifacts
