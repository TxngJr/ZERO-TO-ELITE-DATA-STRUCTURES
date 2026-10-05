# Chapter 037 — Disjoint Set / Union-Find

## Goal

Disjoint Set Union (DSU) หรือ Union-Find เก็บ partition ของสมาชิกออกเป็น disjoint components และรองรับสอง operation หลัก:

- find(x): หา representative/root ของ component ที่มี x
- union(a,b): รวมสอง components

บทนี้ใช้:
- union by size
- path compression
- component count
- component size
- structural validator

## Partition Model

Universe:

    {0,1,2,3,4,5}

Possible partition:

    {0,2,5}
    {1,4}
    {3}

ทุก element อยู่ใน exactly one set.

DSU ไม่ได้เก็บสมาชิกของแต่ละ set เป็น list โดยตรง แต่ใช้ rooted forest.

## Forest Representation

Each element stores:

    parent[i]

Root invariant:

    parent[root] == root

Each root also stores component size.

Example:

    5 -> 2 -> 0
              ^
              |
              0

After path compression on find(5):

    5 ----> 0
    2 ----> 0
    0 ----> 0

Logical partition unchanged; representation becomes flatter.

## Find

Basic find follows parent pointers until root.

Path compression:
after finding root, redirect visited nodes toward root.

This makes future finds faster.

## Union by Size

To merge roots ra and rb:
- attach smaller tree under larger root
- add sizes

Without balancing, repeated unions can create long chains.

Union by size guarantees tree depth O(log n) even before path compression.

## Combined Complexity

With both:
- union by size/rank
- path compression

A sequence of m operations on n elements costs approximately:

    O(m * alpha(n))

where alpha is inverse Ackermann function.

For any practical n, alpha(n) is tiny.

Important:
this is an amortized bound over operation sequences, not a statement that each low-level pointer traversal is literally constant.

## Connected

Two elements are in same component iff:

    find(a) == find(b)

## Component Count

Initially:

    components = n

Every successful union of distinct roots decrements component count by one.

Union inside same component does not change it.

## Component Size

Only roots own authoritative size metadata.

To query size(x):
1. find root
2. return size[root]

Non-root size slots are not semantically meaningful.

## Applications

- Kruskal minimum spanning tree
- dynamic connectivity under edge additions
- image/region labeling
- equivalence classes
- clustering
- offline connectivity
- cycle detection in undirected edge streams

DSU does not support arbitrary split efficiently.

## What DSU Cannot Do Well

After union:
there is no cheap operation to separate a component again.

For fully dynamic connectivity, different structures are required.

## Correctness Invariants

1. each parent index is valid
2. parent graph contains no directed cycle except root self-loop
3. every element reaches exactly one root
4. root size equals number of elements reaching that root
5. sum root sizes = n
6. component_count equals number of roots

## Complexity

| Operation | With size + compression |
|---|---:|
| create | Theta(n) |
| find | O(alpha(n)) amortized |
| connected | O(alpha(n)) amortized |
| union | O(alpha(n)) amortized |
| component_size | O(alpha(n)) amortized |
| validate | O(n^2 log n) teaching validator upper bound |
| storage | Theta(n) |

## Files

- src/int_dsu.*
- tests/test_dsu.c
- examples/dsu_demo.c
- benchmarks/dsu_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
