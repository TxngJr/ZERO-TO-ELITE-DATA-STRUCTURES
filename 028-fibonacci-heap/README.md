# Chapter 028 — Fibonacci Heap

## Goal

Fibonacci Heap ต่อจาก Binomial Heap แต่ใช้ **lazy consolidation** เพื่อทำให้ insert/meld ถูกมาก และใช้ mark + cascading cut เพื่อให้ decrease-key มี amortized cost ต่ำ.

## Core Representation

แต่ละ node เก็บ:
- key
- degree
- mark
- parent / child
- left / right

Roots และ children ใช้ circular doubly linked lists.

Heap เก็บ:
- min pointer
- size

Singleton list มี left/right ชี้ตัวเอง.

## Lazy Strategy

Binomial Heap consolidate degrees หลัง union.
Fibonacci Heap ยอมให้ root degrees ซ้ำชั่วคราว.

Insert:
- สร้าง singleton
- splice เข้า root list
- update min

Actual cost:
    Theta(1)

Meld:
- concatenate root lists
- choose smaller min
- sum sizes
- source becomes empty

Actual cost:
    Theta(1)

## Peek

Direct min pointer:

    Theta(1)

## Extract-Min

1. promote all children of min to roots
2. clear parent pointers and marks
3. remove min
4. consolidate equal-degree roots
5. recompute min
6. free removed node

Actual worst case can be O(n) when root list is large.
Amortized:

    O(log n)

## Consolidation

Roots of same degree are linked:
smaller key remains root, larger key becomes child.

Unlike Binomial Heap, degree uniqueness is restored lazily during extract-min rather than maintained after every insert/meld.

## Decrease-Key

Precondition:
the node handle must be a **live handle belonging to this heap**.

This API intentionally does not perform an O(number-of-roots) membership scan, because such a guard could destroy the classical O(1) amortized decrease-key bound when the lazy root list is large.

If new_key > old_key:
    reject

If new key violates parent order:
- cut node to root list
- cascading-cut old parent

Update min as needed.

## Mark and Cascading Cut

A non-root node starts unmarked.

After it loses one child:
    mark=true

If it later loses another child under the same parent:
- cut it to root list
- continue cascading upward

Roots are always unmarked.

This protects the degree/subtree-size relationship needed for logarithmic maximum degree.

## Delete by Handle

Precondition:
the node handle must be a **live handle belonging to this heap**.

If node is non-root:
- cut it to root list
- perform cascading cut on old parent

Then temporarily select that exact root as the removal target and run extract-min core.

This avoids requiring INT_MIN as a sentinel and supports the full int key domain.

After deletion/extraction, that handle is dangling and must never be reused.

## Degree Bound

Because cascading cuts limit child losses, subtree size grows at least Fibonacci-like with degree.

Therefore maximum degree:

    O(log n)

This bounds consolidation work amortized.

## Potential Intuition

Classic potential is based approximately on:

    Phi = number_of_roots + 2 * number_of_marked_nodes

Cuts may be expensive in one operation, but roots/marks represent stored structural potential from prior operations.

Formal amortized proofs return in Chapter 131.

## Complexity

| Operation | Worst actual | Amortized |
|---|---:|---:|
| peek-min | Theta(1) | Theta(1) |
| insert | Theta(1) | O(1) |
| meld | Theta(1) | O(1) |
| decrease-key | O(n) | O(1) |
| extract-min | O(n) | O(log n) |
| delete(handle) | O(n) | O(log n) |
| validate | Theta(n) | Theta(n) |

Classical handle-operation bounds assume valid live handles from the correct heap.

## Fibonacci vs Binomial Heap

Binomial:
- eager degree consolidation
- meld O(log n)
- simpler structural invariants

Fibonacci:
- lazy roots
- O(1) meld/insert
- O(1) amortized decrease-key
- more pointers, marks and implementation complexity
- weaker locality and often larger constants

## Practical Perspective

Fibonacci Heap is essential theoretically, especially in graph-algorithm analysis, but simpler heaps may be faster in real software due to allocation, pointer chasing, cache misses and constants.

Measure real workloads.

## Files

- src/int_fibonacci_heap.*
- tests/test_fibonacci_heap.c
- examples/fibonacci_heap_demo.c
- benchmarks/decrease_key_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
