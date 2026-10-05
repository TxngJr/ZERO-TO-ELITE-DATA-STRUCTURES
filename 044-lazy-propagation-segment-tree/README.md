# Chapter 044 — Lazy Propagation Segment Tree

## Goal

Chapter 043 ทำ point update ได้ O(log n) แต่ถ้าต้อง:

    add delta ให้ทุก element ใน [l,r)

แบบ naive จะต้องแตะ O(length) leaves.

Lazy Propagation เพิ่ม **deferred tag** ที่ node เพื่อให้ range update และ range query ทำได้ O(log n).

Concrete implementation รองรับ:
- range add
- range sum
- range min
- point get

ทุกช่วงเป็น half-open [left,right).

## Lazy Tag Meaning

For node interval [l,r):

    lazy[node] = delta

หมายความว่า delta ถูกใช้กับ aggregate ของ node นี้แล้ว แต่ยังอาจไม่ได้ push ลง children.

Node stores:

    sum[node]
    min[node]

หลัง apply delta:

    sum += delta * (r-l)
    min += delta
    lazy += delta

## Deferred Work

Suppose node [0,8) receives +5.

Instead of visiting eight leaves:
- update root sum/min
- store lazy=+5
- stop

Only when a later partial update needs children do we push +5 downward.

This converts repeated range work into metadata.

## Push

Before descending through a node with lazy tag:

    apply(left child, lazy)
    apply(right child, lazy)
    lazy[node] = 0

Logical values do not change during push.
Only representation changes.

This is similar in spirit to path compression / deferred storage transformations:
the abstract state stays the same while representation is reorganized.

## Pull

After partial update returns:

    sum[node] = sum[left] + sum[right]
    min[node] = min(min[left], min[right])

because push cleared node lazy before descent.

## Const Query Without Push

Teaching query functions are const.

They do not mutate/push the tree.

Instead they carry deferred ancestor delta:

    carry

For a fully covered node:

    actual_sum =
        sum[node] + carry * interval_length

    actual_min =
        min[node] + carry

For child recursion:

    child_carry = carry + lazy[node]

This shows that lazy state is an alternate representation of the same logical array.

## Structural Invariant

For internal node with interval length L:

    sum[node]
      = sum[left] + sum[right] + lazy[node] * L

    min[node]
      = min(min[left], min[right]) + lazy[node]

Why?

Children do not necessarily include this node's deferred tag.

For leaf:

    sum[node] == min[node]

## Range Add Cases

Update Q=[ql,qr), node N=[l,r):

No overlap:
    return

Full cover:
    apply lazy delta at node

Partial:
    push current lazy
    recurse children
    pull

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n) |
| range add | O(log n) |
| range sum | O(log n) |
| range min | O(log n) |
| point get | O(log n) |
| validate | Theta(n) |
| storage | Theta(n) |

Standard range update/query touches O(log n) canonical boundary nodes, with fully covered subtrees handled in O(1).

## Lazy Composition

For range-add tags:

    add(a) then add(b)
    = add(a+b)

Tag composition is associative and simple.

More advanced lazy trees may combine:
- assignment
- addition
- affine transforms
- min/max constraints

Composition order then becomes critical.

## Integer Contract

All values, tags and aggregated sums use int64_t.

Caller must keep intermediate and final results representable in int64_t.

## Files

- src/lazy_segment_tree.*
- tests/test_lazy_segment_tree.c
- examples/lazy_demo.c
- benchmarks/lazy_vs_naive.c
- standard chapter learning artifacts
