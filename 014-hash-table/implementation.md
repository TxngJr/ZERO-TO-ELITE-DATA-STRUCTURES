# Implementation — IntIntHashTable

## Entry

    key
    value
    next

## Table

    Entry **buckets
    size_t bucket_count
    size_t size

Initial bucket count:
    8

Growth:
    double bucket count when next insertion would push load above 0.75.

## Hashing Signed Int

The int key is converted through fixed-width signed/unsigned representation and fed to hash_u64_mix.

The exact mapping is internal; equality remains ordinary int equality.

## Insert

Search bucket first.

If key exists:
    update value
    no resize
    no size change

If new:
    ensure load-factor capacity
    recompute bucket after any resize
    allocate node
    link at bucket head
    size++

## Remove

Uses pointer-to-pointer:

    Entry **link

This lets code update either:
- bucket head
- predecessor->next

without separate cases.

## Validate

Validator:
- walks each chain with a global count bound
- checks every entry hashes to current bucket
- checks duplicate keys within chain/table by lookup/count logic
- confirms total nodes==size
