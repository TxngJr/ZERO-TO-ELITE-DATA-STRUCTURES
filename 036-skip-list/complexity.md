# Complexity — Skip List

Expected under random-level assumptions:

contains:
    O(log n)

insert:
    O(log n)

remove:
    O(log n)

range:
    O(log n + k)

Worst-case:
    O(n)

Space:
expected O(n) forward pointers for fixed p<1.

Theoretical allocated pointer cap:
    O(n * max_level)

Actual flexible-array allocation only pays each node's sampled level.
