# Theory — Counting Bloom Filter

Replacing each Bloom bit with a counter records how many hash contributions currently touch that position.

Insert increments k counters. Valid deletion decrements the same k counters.

Because collisions merge contributions, counters cannot identify which key contributed. Therefore deletion correctness requires an external guarantee that the removed key occurrence is legitimate.
