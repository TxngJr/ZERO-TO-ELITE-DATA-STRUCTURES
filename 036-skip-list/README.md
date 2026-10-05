# Chapter 036 — Skip List

## Goal

Skip List เป็น ordered probabilistic data structure ที่แทน balanced tree ด้วยหลายระดับของ linked lists.

Level 0:
    contains every key in sorted order

Higher levels:
    contain sampled subsets that act as express lanes.

Expected search/insert/delete:

    O(log n)

Worst-case:

    O(n)

บทนี้สร้าง unique integer Skip List ด้วย probability p=1/2 และ deterministic teaching PRNG.

## Mental Model

Level 3:
    H -------- 20 ---------------- 70

Level 2:
    H ---- 10 - 20 -------- 50 --- 70

Level 1:
    H - 5 - 10 - 20 - 30 - 50 --- 70

Level 0:
    H - 5 - 10 - 20 - 30 - 40 - 50 - 60 - 70

Search key 60:
- move right while next key < 60
- when next would overshoot, drop one level
- repeat

## Why It Works

Each inserted node gets random height.

With p=1/2:

Probability node reaches level k approximately:

    (1/2)^(k-1)

Expected node count by level decreases geometrically.

So high levels are sparse shortcuts.

Expected maximum useful height:

    O(log n)

## Random Level

Teaching algorithm:

    level = 1
    while level < max_level and random_coin == heads:
        level++

PRNG is deterministic for reproducible tests.

It is:
- not cryptographic
- not adversarially secure

Expected-complexity claims assume random levels are sufficiently independent from key order.

## Search

Start:
- head
- highest active level

At each level:
while next exists and next.key < target:
    move right

Then drop one level.

At level 0:
check equality.

Expected:

    O(log n)

## Insert

Need predecessor at every level:

    update[level]

Search top-down while recording last node before insertion position.

If key already exists:
    return false

Generate random node level.

If new level exceeds current active levels:
- predecessors for new levels are head
- increase current_level

Splice new node into levels:

    0 .. node_level-1

## Delete

Build update array exactly like insert.

If target exists:
for every level target participates in:
    bypass target

Then reduce current_level while top head pointer is NULL.

Free node.

## Flexible-Array Node

C representation:

    key
    level
    next[]

Node allocation includes exactly as many forward pointers as its level requires.

This avoids max_level pointers per ordinary node.

Head allocates max_level pointers.

## Ordered Set Semantics

This implementation stores unique integers.

Duplicate insert:
    false

Range traversal:
walk level 0 from first key >= low until key > high.

Unlike Trie:
ordering is numeric key order rather than symbol paths.

## Skip List vs Balanced BST

Balanced BST:
- deterministic O(log n) height guarantee
- rotations/recoloring
- pointer tree

Skip List:
- simpler local pointer splices
- expected O(log n)
- potentially easy concurrent variants
- multiple forward pointers
- worst-case O(n)

## Skip List vs B+ Tree

Skip List:
- memory-oriented
- fine-grained nodes
- probabilistic

B+ Tree:
- page-oriented high fanout
- designed for block/storage locality
- leaf chain supports ranges

## Skip List in LSM MemTables

Skip List is a natural MemTable candidate because:
- ordered
- supports point lookup
- supports sorted iteration needed for flush
- inserts are expected O(log n)
- implementations can be made concurrent

This fixes the major pedagogical simplification in Chapter 035 where sorted-array MemTable insert costs O(M).

## Range Query

Find first node >= low via top-down traversal.

Then level-0 scan:

    while key <= high:
        emit

Expected:

    O(log n + k)

where k = returned items.

## Maximum Level

Finite max_level prevents unbounded node pointer arrays.

If n grows far beyond intended scale:
- height saturates
- performance trends toward longer bottom-level traversals

Production implementations select max level from expected capacity/probability.

## Probability Choice

p=1/2:
- more upper-level nodes
- more pointers
- shorter expected hops

Smaller p:
- fewer pointers
- fewer tall nodes
- potentially more horizontal steps

No universally optimal p independent of workload/cache/layout.

## Complexity

| Operation | Expected | Worst |
|---|---:|---:|
| contains | O(log n) | O(n) |
| insert | O(log n) | O(n) |
| remove | O(log n) | O(n) |
| range k results | O(log n + k) | O(n) |
| storage | O(n) expected pointers | O(n * max_level) theoretical cap |

## Files

- src/int_skip_list.*
- tests/test_skip_list.c
- examples/skip_list_demo.c
- benchmarks/skip_list_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
