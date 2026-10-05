# Pitfalls — Bitset

- shifting by 64
- calling ctz on zero
- forgetting padding-bit mask
- confusing bit_count and word_count
- allowing mismatched lengths in binary operations
- using signed shifts
- counting padding bits
- off-by-one around indices 63/64/65
