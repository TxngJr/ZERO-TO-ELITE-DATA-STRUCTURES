# Lab 014 — Collide, Resize, Rehash

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch014_hash_table_tests

## Part A — Force collisions

Find several integer keys mapping to same bucket at bucket_count=8.

Insert them and verify all remain retrievable.

## Part B — Update

Insert key 42 twice with different values.

Verify:
- size increases only first time
- get returns new value

## Part C — Resize

Insert enough keys to trigger several resizes.

Use validator after each insertion.

Explain why old bucket indexes cannot be reused after bucket_count changes.

## Part D — Differential Test

Randomized test compares Hash Table with a simple reference key/value array.

Reference is slower but easy to reason about.

## Part E — Benchmark

    ./build/ch014_hash_table_benchmark

Compare hash lookup with linear-map lookup over growing n.

Do not extrapolate one timing ratio universally.
