# Theory — Bitset

Packing boolean state into machine words trades simple byte addressing for:
- much lower memory
- better cache density
- word-parallel boolean operations

The representation mapping is exact:

    logical bit i
      -> words[i >> 6]
      -> mask 1ULL << (i & 63)

Correctness requires padding bits outside the logical domain to remain zero.
