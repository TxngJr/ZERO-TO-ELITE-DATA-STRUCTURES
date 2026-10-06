# Theory — External-Memory / I/O Model

When data is much larger than fast memory, asymptotic CPU comparisons can hide the dominant cost: transfers between storage levels.

Classic external-memory model uses:
- M = fast-memory capacity
- B = transfer block size
- N = data items

Goal: minimize I/Os, not merely comparisons.

Examples:
- sequential scan: Θ(N/B) transfers
- binary search in a flat file: O(log N) random transfers without a block index
- B-tree search: O(log_B N) block transfers under standard assumptions

Teaching set keeps a small directory in RAM and models data blocks separately.
