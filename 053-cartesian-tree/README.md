# Chapter 053 — Cartesian Tree

## Goal

Cartesian Tree ของ sequence ผสานสอง invariants:

1. **inorder order**
   - inorder traversal คืน index เดิม 0..n-1

2. **heap order**
   - parent value <= child value สำหรับ min Cartesian Tree

ดังนั้น root คือ minimum ของ sequence ทั้งช่วง.

บทนี้สร้าง static min Cartesian Tree ใน:

    Theta(n)

ด้วย monotonic stack.

## Sequence Defines the Tree

Given values:

    [3, 1, 4, 0, 2]

Cartesian Tree must preserve:

    inorder indices = 0,1,2,3,4

and min-heap property.

Global minimum at index 3 becomes root.

Left subtree is Cartesian Tree of:

    [3,1,4]

Right subtree is Cartesian Tree of:

    [2]

## Tie Rule

Duplicates are allowed.

Builder pops stack only when:

    previous_value > current_value

not when equal.

Therefore equal values keep earlier indices above/later in stable order.

For RMQ, ties resolve to the **leftmost minimum**.

## Linear-Time Monotonic Stack Build

For each index i:

1. pop larger values from stack
2. last popped subtree becomes left child of i
3. current stack top, if any, becomes parent of i
4. i becomes right child of that stack top
5. push i

Each index:
- pushed once
- popped at most once

Total:

    Theta(n)

## Why Inorder Is the Original Sequence

The stack construction only rewires nodes while preserving:

    all indices in left subtree < node.index
    all indices in right subtree > node.index

Therefore inorder traversal returns increasing indices.

## RMQ Connection

For query:

    [left,right)

If current node index:
- < left -> query lies in right subtree
- >= right -> query lies in left subtree
- inside range -> current node is the minimum answer

Why the final case works:

A node is <= every descendant by heap order.
If node itself belongs to the query, no descendant inside the query can be smaller.

Complexity:

    O(height)

Important:
Cartesian Tree is **not balanced**.

Worst case for sorted input:

    height = n-1 edges

so direct tree RMQ can be:

    O(n)

## Cartesian Tree + LCA

Classic result:

    RMQ in array
    <-> LCA in Cartesian Tree

With:
- Euler tour
- depth array
- LCA preprocessing

RMQ can be much faster.

Chapter 048 Sparse Table already supplied one building block for static RMQ.
This chapter focuses on the Cartesian representation itself and the RMQ/LCA connection.

## Height Behavior

Random sequences often produce much smaller height.

But monotonic sequences produce chains:

Ascending min Cartesian Tree:

    0
     \
      1
       \
        2
         \
          ...

Descending sequence produces the opposite chain.

Never claim O(log n) tree height without balancing.

## Array Representation

Node identity is exactly the original sequence index.

For every index:
- parent[index]
- left[index]
- right[index]

Missing link:

    SIZE_MAX

This avoids per-node allocation and makes inorder-index semantics explicit.

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n) |
| root lookup | O(1) |
| parent/child lookup | O(1) |
| direct range minimum | O(h), worst O(n) |
| height | O(n) inspection |
| storage | Theta(n) |

## Files

- src/int_cartesian_tree.*
- tests/test_cartesian_tree.c
- examples/cartesian_demo.c
- benchmarks/cartesian_shape_benchmark.c
- standard learning artifacts
