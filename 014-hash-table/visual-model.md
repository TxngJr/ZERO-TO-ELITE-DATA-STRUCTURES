# Visual Model — Separate Chaining

bucket_count=4

buckets[0] -> [8:80] -> [12:120] -> NULL
buckets[1] -> NULL
buckets[2] -> [6:60] -> NULL
buckets[3] -> [7:70] -> NULL

Collision:
8 and 12 may share bucket while keys remain distinct.

## Rehash 4 -> 8

Each existing entry:
- recompute bucket using new bucket_count
- relink into new bucket array

old chain grouping can split apart.

## Lookup

key
 |
hash
 |
bucket index
 |
walk only that chain
 |
actual key comparisons
