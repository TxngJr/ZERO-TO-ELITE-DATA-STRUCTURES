# Invariants — IntBitmap

1. word_count == ceil(universe_size/64)
2. padding bits are zero
3. cardinality == total popcount(words)
4. cardinality <= universe_size
5. membership index must be < universe_size
6. set algebra requires identical universe sizes
