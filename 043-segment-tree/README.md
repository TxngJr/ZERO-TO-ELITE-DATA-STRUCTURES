# Chapter 043 — Segment Tree

## Goal

Segment Tree เก็บ aggregate ของช่วงข้อมูลเพื่อให้:
- point update
- range query

ทำได้แบบ logarithmic.

บทนี้ใช้ array คงที่ขนาด n และเก็บสอง aggregate พร้อมกัน:
- range sum
- range minimum

ทุก query ใช้ half-open interval:

    [left, right)

## Core Idea

Array:

    a[0..n)

Root represents:

    [0,n)

Each internal node splits interval:

    [l,r) -> [l,m) + [m,r)

where:

    m = l + (r-l)/2

Each node caches:

    sum(node)
    min(node)

For internal node:

    sum = sum(left) + sum(right)
    min = min(min(left), min(right))

## Why Associativity Matters

Range query combines partial answers.

For sum:

    (a+b)+c = a+(b+c)

For min:

    min(min(a,b),c) = min(a,min(b,c))

Associativity allows interval pieces to be combined without changing the result.

This connects Segment Trees to the general **monoid** viewpoint:
- associative combine operation
- identity value

The concrete C implementation keeps sum/min together for clarity.

## Query Cases

For query Q=[ql,qr) and node interval N=[l,r):

1. No overlap:
       N ∩ Q = empty
2. Full cover:
       N subset of Q
       return cached aggregate
3. Partial overlap:
       query children and combine

Teaching implementation avoids using fake min sentinels by returning a boolean for whether a partial minimum exists.

## Point Update

To set:

    a[index] = value

descend one root-to-leaf path.

At leaf:
- replace sum/min with value

On return:
- recompute ancestors

Only O(log n) nodes change.

## Build

Bottom-up logical recurrence visits each tree node once:

    Theta(n)

Do not confuse this with n independent point updates:

    O(n log n)

The recursive build is linear.

## Array-backed Layout

This implementation stores tree nodes in arrays indexed like:

    node
    left  = node*2
    right = node*2+1

The arrays are allocated at about 4n slots.

Advantages:
- simple ownership
- good locality
- no per-node malloc

Disadvantage:
- space proportional to n even if coordinate universe is mostly empty

Chapter 045 solves that with a dynamic sparse tree.

## Invariants

For every represented interval:

Leaf:
    sum[node] = min[node] = a[index]

Internal:
    sum[node] = sum[left] + sum[right]
    min[node] = min(min[left], min[right])

Root:
    sum/root minimum represent entire array.

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n) |
| point set | O(log n) |
| point get | O(log n) |
| range sum | O(log n) canonical Segment Tree bound |
| range min | O(log n) |
| validate | Theta(n) |
| storage | Theta(n) |

For an arbitrary interval, a standard binary Segment Tree visits O(log n) canonical boundary structure per level overall, giving O(log n) query time.

## Integer Contract

Aggregates use int64_t.

Caller must keep values/updates such that every stored sum fits int64_t.
This chapter focuses on data-structure mechanics rather than arbitrary-precision arithmetic.

## Files

- src/int_segment_tree.*
- tests/test_segment_tree.c
- examples/segment_tree_demo.c
- benchmarks/segment_tree_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
