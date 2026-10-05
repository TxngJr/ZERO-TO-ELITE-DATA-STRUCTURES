# Visual Model — Bloom Filter

Bit array:

    0 0 1 0 1 1 0 0 1 ...

Key A hashes to positions 2,4,8.

Query B hashes to 2,5,8:
- all three are 1 -> maybe present

Query C hashes to 1,4,8:
- position 1 is 0 -> definitely absent
