# Theory — Hash Table

## Separate Chaining as Array of Lists

Hash table is composition of data structures:
- array gives O(1) bucket selection
- linked chains handle collisions

This is a recurring systems pattern: combine simple structures to obtain desired behavior.

## Expected Chain Length

If n keys distribute uniformly among m buckets:

    alpha=n/m

expected bucket occupancy is alpha.

A lookup sees expected bounded chain work when alpha is kept bounded.

## Worst Case

No hash table magically prevents a bad collision set.

If all n keys map one bucket:
lookup becomes linear.

Therefore complexity claims must state assumptions.

## Rehash Atomicity

A robust resize should avoid destroying old representation before new bucket array allocation succeeds.

Implementation:
1. allocate new bucket array
2. relink existing nodes into it
3. free old bucket-array storage
4. commit new metadata

Node allocations themselves are reused.

## Iteration Ordering

Bucket/chaining order is representation-specific and can change after resize.

Therefore map iteration order should be considered unspecified unless API explicitly promises order.
