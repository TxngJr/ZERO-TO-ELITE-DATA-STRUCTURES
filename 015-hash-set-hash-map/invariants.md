# Invariants — Hash Set / Hash Map

## IntHashSet

All invariants come from underlying IntIntHashTable.
Publicly:
- every value occurs at most once
- size equals number of distinct stored values

## StringIntHashMap

1. size + tombstones <= capacity
2. EMPTY slot has key==NULL
3. TOMBSTONE slot has key==NULL
4. OCCUPIED slot has key!=NULL
5. every OCCUPIED key is reachable by its linear-probe sequence before an EMPTY
6. no duplicate equal keys
7. size counts OCCUPIED slots
8. tombstones counts TOMBSTONE slots
9. map owns every OCCUPIED key allocation
10. cached hash equals hash of stored key

Rehash preserves every logical key/value mapping and eliminates tombstones.
