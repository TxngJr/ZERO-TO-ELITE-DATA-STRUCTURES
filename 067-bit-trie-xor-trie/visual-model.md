# Visual Model — 4-bit Example

Values:

    0011
    0101
    1110

Query = 0110.

For maximum XOR query MSB is 0, so prefer stored MSB 1 when that subtree is live. Once XOR MSB becomes 1, lower bits cannot overturn that advantage.
