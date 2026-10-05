# Lab 015 — Set Semantics and Tombstones

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch015_hash_set_map_tests

## Part A — Set Idempotence

Add:
    10,20,10,30,20

Verify size=3.

Explain why duplicate add is not an error.

## Part B — Tombstone Chain

Create colliding keys in StringIntHashMap by searching for strings with same home slot under current capacity.

Delete the earliest one and verify later colliding keys remain findable.

Explain why EMPTY would fail.

## Part C — Ownership

Create mutable char buffer:
1. put(buffer, value)
2. modify original buffer
3. verify stored original key remains findable

This demonstrates owned-key copy.

## Part D — Differential Tests

Tests compare:
- IntHashSet vs boolean reference domain
- StringIntHashMap vs small reference array model

## Part E — Benchmark

    ./build/ch015_set_lookup_benchmark

Compare hash-set membership with linear-array membership as n grows.
