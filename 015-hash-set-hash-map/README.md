# Chapter 015 — Hash Set / Hash Map

## Goal

Set และ Map เป็น ADTs คนละแบบ แต่ hashing สามารถเป็น representation ของทั้งคู่ได้.

บทนี้สร้าง:
- IntHashSet โดย reuse IntIntHashTable จาก Chapter 014
- StringIntHashMap แบบ Open Addressing + Linear Probing + Tombstones

หลังจบบทนี้คุณควร:
- แยก Set กับ Map semantics
- เข้าใจ key uniqueness
- reuse Map เพื่อ implement Set
- เข้าใจ open addressing/linear probing
- อธิบาย primary clustering
- เข้าใจ tombstones และ why deletion cannot mark EMPTY
- implement resize/rehash
- understand owned string keys
- distinguish borrowed lookup keys from owned stored keys
- compare chaining vs open addressing

## 1. Hash Set ADT

Set stores unique values:

    add(x)
    contains(x)
    remove(x)
    size()

Adding x twice still leaves one x.

## 2. Hash Map ADT

Map stores unique keys associated with values:

    put(k,v)
    get(k)
    contains_key(k)
    remove(k)
    size()

put existing key updates value.

## 3. Set Can Reuse Map

A Set can be represented as:

    Map<Element, Dummy>

IntHashSet wraps IntIntHashTable and stores dummy value 1.

This demonstrates ADT != representation.

## 4. Open Addressing

Entries live directly in slot array.

Linear probing:

    index0 = hash % capacity
    index1 = next(index0)
    index2 = next(index1)

Pros:
- contiguous storage
- fewer per-entry allocations
- locality

Costs:
- clustering
- tombstone complexity
- must maintain spare slots

## 5. Slot States

Each slot:
- EMPTY
- OCCUPIED
- TOMBSTONE

Lookup stops at EMPTY but must continue through TOMBSTONE.

## 6. Why Delete Cannot Mark EMPTY

If B collided behind A, deleting A and turning its slot EMPTY would make lookup B stop too early.

TOMBSTONE preserves probe continuity.

## 7. Tombstone Reuse

Insertion remembers first tombstone.
If key not found before EMPTY, reuse earliest tombstone.

## 8. Clustering

Linear probing creates contiguous clusters.
High load increases expected probe length.

Other strategies include quadratic probing, double hashing, Robin Hood and SwissTable-like designs.

## 9. Load Factor

Open addressing must keep spare slots.

This implementation grows before a new live entry would exceed roughly 75% load.

Tombstones also consume probe space, so implementation may rehash the same capacity to clean them.

## 10. String-Key Ownership

put accepts borrowed const char*.

For a new key:
- map makes owned copy
- caller may modify/free original after successful put

remove/free releases owned key.

## 11. Cached Hash

Each occupied slot stores key, value and 64-bit hash.

Lookup checks cached hash before strcmp.

## 12. Rehash

Fresh slot array starts EMPTY.

For each OCCUPIED old slot:
- use cached hash
- probe in new capacity
- move key pointer/value/hash
- do not duplicate key again

Tombstones disappear.

## 13. Complexity

Under good hashing and bounded load:

Set operations:
    expected Θ(1)

String map:
    expected hash cost Θ(L) plus bounded probe/equality work

Worst cases can degrade toward linear probing/chaining scans under pathological collisions.

## 14. Chaining vs Open Addressing

Chaining:
- deletion simple
- alpha may exceed 1
- pointer allocations/chasing

Open addressing:
- contiguous slots
- alpha<1
- tombstones/clustering
- resize policy critical

Neither is universally best.

## 15. Iteration Order

Order is unspecified in these implementations.

Resize and collision history can change physical order.

## 16. Set Algebra

Set naturally supports union/intersection/difference/subset.

Expected costs can approach total input sizes under expected constant-time membership assumptions.

## Files

- src/int_hash_set.*
- src/string_int_hash_map.*
- tests/test_hash_set_map.c
- examples/hash_set_map_demo.c
- benchmarks/set_lookup_benchmark.c
- theory/visual/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
