# Chapter 058 — Spatial Hashing

## Goal

Spatial Hashing แบ่งพื้นที่ 2D เป็น grid cells แล้วใช้ hash table map:

    (cell_x, cell_y) -> points in that cell

รองรับ:
- dynamic point insertion
- rectangle count/report
- sparse unbounded coordinates
- negative coordinates
- structural validation

Point:
    (x, y, id)

Cell size:
    positive int64_t

## Cell Mapping

For cell size S:

    cell_x = floor(x / S)
    cell_y = floor(y / S)

คำว่า floor สำคัญมาก.

C integer division truncates toward zero:

    -1 / 10 == 0

แต่ spatial grid ต้องการ:

    floor(-1/10) == -1

Implementation จึงมี explicit floor_div.

## Sparse Grid

เราไม่ allocate grid ทั้งโลก.

Hash table เก็บเฉพาะ cells ที่มี points:

    occupied cells only

เหมาะกับ:
- collision broad phase
- particles
- game entities
- nearby-neighbor candidate lookup
- sparse spatial simulations

## Hash Table Representation

Table:
- power-of-two bucket count
- bucket head stores cell index
- separate chaining via cell.next

Cell arena:
- stable integer index
- cell coordinates
- dynamic point bucket

เมื่อ load factor ของจำนวน cells สูง:
- table buckets ถูก rehash
- cell point buffers ไม่ย้าย ownership

## Rectangle Query

For half-open rectangle:

    [x_low,x_high) × [y_low,y_high)

compute touched cell coordinates:

    first_x = floor_div(x_low,S)
    last_x  = floor_div(x_high-1,S)

and same for y.

Enumerate only those cells.
For each existing cell:
- scan its points
- apply exact rectangle predicate

Grid cells are candidate filters; exact point test is still required on boundary cells.

## Complexity

Let:
- C = number of cells touched by query
- P = number of candidate points inside those cells

Expected with a healthy hash table:

Insert:
    expected O(1)

Query:
    expected O(C + P)

Worst case:
    can degrade with hash collisions or huge query area

Storage:
    Theta(points + occupied cells + hash buckets)

## Spatial Hashing vs Quadtree

Spatial Hashing:
- fixed cell size
- flat hash lookup
- simple dynamic updates
- performance depends strongly on cell size

Quadtree:
- adaptive hierarchical resolution
- more structure/node overhead
- better for uneven spatial density in some workloads

## Choosing Cell Size

Too small:
- many empty/tiny occupied cells
- query touches many cells

Too large:
- each cell contains many candidates
- exact filtering work grows

A common heuristic:
    choose cell size near interaction/search radius

but workload measurement should decide.

## Files

- src/int_spatial_hash.*
- tests/test_spatial_hash.c
- examples/spatial_hash_demo.c
- benchmarks/spatial_hash_benchmark.c
- standard learning artifacts
