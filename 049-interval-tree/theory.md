# Theory — Interval Tree

## Augmented search tree

An Interval Tree is not a new ordering rule.

It is a balanced search tree plus metadata.

The BST key answers:
    where intervals are ordered

The augmentation answers:
    whether a subtree can still contain a useful interval

## Correctness of left pruning

If:

    left.max_high <= query.low

then for every interval [a,b) in left:

    b <= query.low

so:

    query.low < b

is false.

Therefore no interval in the left subtree overlaps the query.

## Why low ordering helps right pruning

If current.low >= query.high, every interval in the right subtree has:

    low >= current.low >= query.high

so:

    low < query.high

is false.

Thus current/right can be skipped for all-overlap traversal.
