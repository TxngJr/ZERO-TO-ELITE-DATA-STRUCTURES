# Implementation — IntBitset

State:
- bit_count
- word_count
- uint64_t *words

Important helper:

    mask_padding()

keeps unused high bits of the final word zero.

Binary bitwise APIs require equal logical bit_count.
