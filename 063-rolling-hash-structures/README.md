# Chapter 063 — Rolling Hash Structures

## Goal

Rolling Hash แปลง substring เป็น fingerprint ที่คำนวณได้เร็วจาก prefix metadata.

บทนี้ implement **double polynomial rolling hash** สำหรับ arbitrary byte text.

รองรับ:
- O(1) substring hash extraction
- collision-aware exact substring equality
- longest common prefix (LCP) by hash + binary search
- Rabin-Karp contains/count/report
- binary byte 0
- structural validation

## Polynomial Prefix Hash

Map byte b to:

    value = b + 1

เพื่อไม่ให้ byte 0 กลายเป็น coefficient 0.

For modulus M and base B:

    prefix[i+1]
      = (prefix[i] * B + value(text[i])) mod M

Power table:

    power[k] = B^k mod M

Hash of half-open substring [l,r):

    H(l,r)
      = prefix[r]
        - prefix[l] * power[r-l]

mod M.

So extraction is O(1).

## Double Hash

ใช้สอง moduli:

    M1 = 1,000,000,007
    M2 = 1,000,000,009

และ base:

    B = 911,382,323

Hash pair:

    (h1,h2)

ลด collision probability ลงมากกว่าการใช้ modulus เดียว.

แต่:

> hash equality is still not a proof of string equality.

## Collision-Aware Exact Equality

API exact-equality ทำ:

1. compare lengths
2. compare double hash
3. if hashes differ -> definitely not equal
4. if hashes equal -> verify bytes with memcmp

ดังนั้น API equality มี correctness แบบ deterministic แม้ theoretical hash collision เกิดขึ้น.

Hash ทำหน้าที่เป็น **fast rejection filter**.

## Rabin-Karp Search

Pattern hash คำนวณครั้งเดียว.

For every text window of same length:
1. extract O(1) double hash
2. if hash differs -> skip
3. if hash matches -> memcmp verify
4. report exact match only

Complexity expected:
    O(n + m + verified candidates * m)

Worst case:
    O(nm)

if many windows collide or equal-prefix candidate verification is frequent.

## Longest Common Prefix

For two suffix/substring starts a,b with max_length L:

Binary search largest length k such that:

    hash(a,a+k) == hash(b,b+k)

For exact deterministic result, after binary-search candidate:
- byte equality can be verified
- if verification ever exposes collision, fallback linear comparison resolves exact LCP

This implementation simplifies correctness by using double-hash binary search and final exact linear verification bounded by max_length.

Complexity:
- typical O(log L) hash probes + O(L) exact verification
- exact deterministic bound O(L + log L)

A purely probabilistic LCP could omit verification and be O(log L).

## Hash Arithmetic

All products fit uint64_t:

    values < ~1e9
    product < ~1e18
    UINT64_MAX ~1.84e19

So modular multiplication does not overflow uint64_t for chosen constants.

## Static Structure

Prefix hash is built for immutable text.

If text changes:
- prefix hashes after mutation point become invalid

Dynamic rolling-hash trees can combine:
- segment trees
- Fenwick trees
- balanced sequence trees

but are outside this chapter.

## Complexity

| Operation | Complexity |
|---|---:|
| build | O(n) |
| raw substring hash | O(1) |
| exact substring equality | O(1) reject, O(length) on hash match |
| Rabin-Karp search | expected near O(n+m), worst O(nm) |
| exact LCP | O(L + log L) here |
| storage | Theta(n) |

## Files

- src/byte_rolling_hash.*
- tests/test_rolling_hash.c
- examples/rolling_hash_demo.c
- benchmarks/rolling_hash_benchmark.c
- standard learning artifacts
