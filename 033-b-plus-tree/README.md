# Chapter 033 — B+ Tree

## Goal

B+ Tree เป็น storage/index tree ที่คล้าย B-Tree แต่แยกบทบาทชัดเจนกว่า:

- actual stored keys อยู่ที่ leaf nodes
- internal keys เป็น separator copies
- leaves เชื่อมกันเป็น linked list
- range scan จึงเดินต่อเนื่องตาม leaves ได้

บทนี้สร้าง Integer B+ Tree แบบ minimum degree t.

## Core Difference from B-Tree

B-Tree:
- key อาจเป็น record key อยู่ใน internal node หรือ leaf
- inorder traversal ต้องผ่านทุกระดับ

B+ Tree:
- logical set อยู่ที่ leaves เท่านั้น
- internal separators ใช้ routing
- leaves linked left-to-right

ดังนั้น range scan:
1. descend to first relevant leaf
2. walk leaf next pointers

## Capacity Convention

This implementation uses:

    max keys per node = 2t-1

For non-root:
- leaf min keys = t-1
- internal min keys = t-1
- internal child count = key_count+1

During split a temporary insertion may create 2t keys.

## Separator Invariant

For internal node:

    keys[i] = minimum key in children[i+1]

Routing rule:

    while key >= keys[i]:
        ++child_index

This means equality goes to the right child where that separator key actually lives in a leaf.

## Search

Search descends internal separators until leaf.

Only leaf key equality means logical membership.

## Insert

Insert into target leaf in sorted unique order.

If leaf overflows:
- split leaf entries into left/right halves
- link right into leaf chain
- propagate right minimum separator to parent

If internal node overflows:
- split child pointers
- separators are recomputed from child minima
- propagate split upward

If root splits:
create a new internal root with two children.

## Why Recompute Separators?

Rather than hand-editing separator keys after every borrow/merge, this teaching implementation defines:

    separator[i] = min(children[i+1])

and recomputes separators locally after structural mutation.

This makes correctness easier to reason about and exposes the fundamental B+ invariant directly.

## Delete

Delete key from leaf.

If non-root leaf drops below t-1 keys:
- borrow one key from left sibling, or
- borrow one key from right sibling, or
- merge leaves and repair leaf next pointer

If internal child underflows:
- borrow a child pointer from sibling, or
- merge child-pointer arrays

After each repair:
- recompute separators in affected nodes
- propagate separator changes upward

## Root Shrink

If internal root reaches zero separators, it has one child.
That child becomes new root.

If root is leaf and becomes empty, tree remains as one empty leaf.

## Leaf Chain

Every leaf has:

    next

Validator checks:
- chain order strictly increasing
- chain visits exactly the same leaves as tree order
- total leaf key count equals tree size

This is what enables range scans without returning to parent nodes.

## Range Query

API:

    range(low, high)

returns keys satisfying:

    low <= key <= high

Algorithm:
1. descend to leaf where low would appear
2. scan within leaf
3. follow next leaves until key > high

Cost:

    O(height + output)

plus in-leaf scan costs.

## B+ Tree and Databases

Common index pattern:
- internal pages contain separators + child page IDs
- leaves contain keys + record IDs or values
- leaves linked for range scans

Real implementations also need:
- page size layout
- latch/concurrency protocol
- WAL/recovery
- prefix compression
- fill factors
- sibling/page IDs
- buffer pool integration

Later storage chapters build on these ideas.

## Complexity

For fixed t:

Search:
    O(log n)

Insert:
    O(log n)

Delete:
    O(log n)

Range:
    O(log n + k)
where k is output count.

Storage:
    Theta(n)

External-memory:
    O(height + output_pages)

## Files

- src/int_bplus_tree.*
- tests/test_bplus_tree.c
- examples/bplus_demo.c
- benchmarks/range_scan_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
