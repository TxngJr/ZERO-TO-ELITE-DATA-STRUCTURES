# Lab 013 — See Collisions

## Build

    cmake -S . -B build
    cmake --build build
    ./build/ch013_hash_tests

## Part A — Hash values

Run:

    ./build/ch013_hash_demo

Change one character of a string and compare output in hex.

Do not conclude cryptographic avalanche from a few examples; this is observation only.

## Part B — Poor modulo mapping

Create keys:

    0,16,32,...,16*99

Use bucket_count=16:
- direct key % 16
- hash_u64_mix(key) % 16

Compare occupancy.

## Part C — Distribution Benchmark

    ./build/ch013_hash_distribution

Inspect:
- empty buckets
- min occupancy
- max occupancy

Try 64, 128 and 257 buckets.

## Part D — Collision Contract

Find two different integers that map to the same bucket for m=8.

Explain why a future Hash Table must compare actual keys after selecting the bucket.
