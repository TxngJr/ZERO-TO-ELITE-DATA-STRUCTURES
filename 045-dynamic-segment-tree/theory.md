# Theory — Dynamic Segment Tree

## Sparse materialization

The complete conceptual Segment Tree exists mathematically over the whole domain.

The implementation materializes only nodes required by nonzero/touched updates.

NULL is therefore a compressed representation of an all-zero subtree.

## Correctness

Leaf:
node sum equals logical value at its coordinate.

Internal:
node sum equals left logical sum plus right logical sum, treating absent children as zero.

By structural induction, every node sum matches its interval.

## Height

Repeated halving of a span U reaches unit intervals after O(log U) levels.

For a uint64_t domain, height is at most about 64 for ordinary nonempty finite intervals.
