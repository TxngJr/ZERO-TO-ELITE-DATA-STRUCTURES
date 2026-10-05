# Complexity — Hash Set / Hash Map

## IntHashSet

Using Chapter 014 chaining table:

add/contains/remove:
    expected Θ(1) under bounded load/uniform-ish hashing
    worst Θ(n)

space:
    Θ(n+m)

## StringIntHashMap

Let L=key length.

Hashing:
    Θ(L)

Probe work:
    expected O(1) slots at controlled load under good hashing

String equality may inspect up to Θ(L) or compared-key length.

Typical operation:
    expected O(L) plus bounded probe/equality factors

Worst collision/clustering:
    O(n * comparison-cost) in pathological cases

Rehash:
    O(n+m_new), excluding no rehash of stored strings because cached hashes are kept

space:
    Θ(capacity + total owned key bytes)

## Set Algebra

For set A,B with hash membership:
union/intersection/difference can often be expected O(|A|+|B|), assuming expected constant-time membership/insertion.
