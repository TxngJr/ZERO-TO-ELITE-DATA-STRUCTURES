# Chapter 056 — Octree

## Goal

Octree ขยาย Quadtree จาก 2D ไปเป็น 3D.

แต่ละ internal node แบ่ง box ออกเป็น:

    2 × 2 × 2 = 8 octants

บทนี้ implement dynamic point-region Octree สำหรับ:

    (x, y, z, id)

รองรับ:
- insert
- axis-aligned box count
- box report
- node statistics
- structural validation

Root domain:

    [x_low,x_high)
    × [y_low,y_high)
    × [z_low,z_high)

## Octant Routing

Midpoints:

    mid_x, mid_y, mid_z

Bit flags:

    east  = x >= mid_x
    north = y >= mid_y
    upper = z >= mid_z

Octant index:

    east
    + 2*north
    + 4*upper

จึงได้ index 0..7.

## Bucketed Leaves

เหมือน Chapter 055:
- leaf เก็บหลาย points
- split เมื่อ bucket เต็ม
- stop splitting เมื่อถึง max_depth หรือ integer cell split ต่อไม่ได้
- coincident 3D points จึงไม่ทำให้ recursive subdivision ไม่สิ้นสุด

## Subtree Pruning

ทุก node มี fixed spatial box และ cached:

    subtree_size

Box query:

- disjoint -> prune
- fully covered -> return subtree_size
- partial -> inspect/recurse

## Why Octrees Matter

ใช้กับ:
- 3D occupancy
- voxel worlds
- point clouds
- collision broad phase
- visibility/spatial culling
- robotics mapping

## Quadtree vs Octree

Quadtree:
    2 dimensions
    4 children

Octree:
    3 dimensions
    8 children

จำนวน children โดยหลักการสำหรับ binary split ใน d dimensions:

    2^d

นี่แสดง curse of dimensionality ด้าน branching factor อย่างง่าย.

## Complexity

ขึ้นกับ point distribution และ tree depth.

Typical spatially useful case:
- insert O(depth)
- query proportional to visited octants + output

Worst case:
    O(n)

Storage:
    O(points + allocated nodes)

## Files

- src/int_octree.*
- tests/test_octree.c
- examples/octree_demo.c
- benchmarks/octree_benchmark.c
- standard learning artifacts
