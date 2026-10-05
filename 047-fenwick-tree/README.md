# Chapter 047 — Fenwick Tree / Binary Indexed Tree

## Goal

Fenwick Tree (Binary Indexed Tree, BIT) เป็น structure ขนาด O(n) สำหรับ prefix aggregates ที่รองรับ:

- point add
- prefix sum
- range sum
- point get
- point set

ใน O(log n).

บทนี้ใช้ int64_t sum และ half-open ranges:

    prefix_sum(end) = sum a[0..end)
    range_sum(l,r)  = sum a[l..r)

## Core Idea

Fenwick Tree ใช้ internal indexing แบบ 1-based.

Each tree[i] stores a suffix block ending at i:

    length = lowbit(i)

where:

    lowbit(i) = i & -i

Conceptually:

    tree[i] =
        sum of original elements
        [i-lowbit(i)+1, i]
        in 1-based indexing

Example:

    i=12 = 1100b
    lowbit(12)=4

tree[12] covers 1-based positions:

    9..12

which corresponds to 0-based:

    [8,12)

## Binary Meaning

lowbit(i) isolates the least-significant set bit.

It determines:
- block size represented by tree[i]
- next ancestor during update
- previous prefix block during query

This is why the structure is called Binary Indexed Tree.

## Prefix Query

To compute prefix sum through internal index i:

    sum = 0
    while i > 0:
        sum += tree[i]
        i -= lowbit(i)

Every step clears one set bit.

Number of iterations:

    O(log n)

## Point Add

To add delta at 0-based index p:

    i = p+1
    while i <= n:
        tree[i] += delta
        i += lowbit(i)

Each visited node is a stored block that contains p.

## Range Sum

Because ordinary addition has an inverse:

    sum[l,r)
      = prefix(r) - prefix(l)

This is an important difference from a generic Segment Tree.

Fenwick Tree is especially natural when the aggregate forms a group-like prefix system where subtraction/inverse exists.

For min/max, this simple subtraction trick does not work generally.

## Point Get

A point is just a unit range:

    a[i] = range_sum(i,i+1)

## Point Set

To set:

    a[i] = value

first read old value:

    old = point_get(i)

then:

    point_add(i, value-old)

Cost remains O(log n).

## Linear Build

A Fenwick Tree can be built in Theta(n), not only O(n log n).

Algorithm:

For i from 1..n:
1. tree[i] += value[i-1]
2. parent = i + lowbit(i)
3. if parent <= n:
       tree[parent] += tree[i]

At the moment tree[i] is propagated, it already contains the correct block sum for its range.

## Invariant

For every internal index i:

    tree[i]
      = sum of a[j]
        for j in
        [i-lowbit(i), i)
        under 0-based half-open notation

More explicitly:

    tree[i] covers
    original indices
    i-lowbit(i) ... i-1

## Fenwick vs Segment Tree

Fenwick:
- O(n) compact array
- small constants
- prefix/range sum elegant
- point add O(log n)
- range sum O(log n)
- less general combine algebra

Segment Tree:
- O(n) but larger constant
- arbitrary associative aggregate
- easier to extend to min/max and lazy propagation
- more flexible interval metadata

Choose by operation set, not by familiarity.

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n) |
| point add | O(log n) |
| prefix sum | O(log n) |
| range sum | O(log n) |
| point get | O(log n) |
| point set | O(log n) |
| storage | Theta(n) |

## Integer Contract

All stored sums are int64_t.

Caller must keep:
- tree block sums
- prefix sums
- range subtraction
- set delta

representable in int64_t.

## Files

- src/int_fenwick_tree.*
- tests/test_fenwick_tree.c
- examples/fenwick_demo.c
- benchmarks/fenwick_benchmark.c
- standard learning artifacts
