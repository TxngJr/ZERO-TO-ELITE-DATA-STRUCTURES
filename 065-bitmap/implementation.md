# Implementation — IntBitmap

State:
- universe_size
- word_count
- uint64_t words[]
- cached cardinality

Mutation helpers calculate old/new popcount for changed words.

All ranges are half-open.
