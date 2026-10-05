# Implementation — IntBitVector

State:
- bit_count
- word_count
- packed uint64_t words
- prefix_ones[word_count+1]

Construction validates each input byte is 0 or 1.

The vector is immutable after construction so prefix metadata cannot become stale.
