# Theory — B-Tree

## Occupancy gives height bound

Every non-root internal node has at least t children.

Therefore the minimum number of keys grows exponentially with height, giving:

    h = O(log_t n)

## Split correctness

Median moves to parent.
All left keys remain below median.
All right keys remain above median.
If internal, child pointers split consistently with key ranges.

## Delete repair principle

Before recursion descends, ensure target child has at least t keys.
Then recursive deletion can remove one key without immediately violating the t-1 minimum.
