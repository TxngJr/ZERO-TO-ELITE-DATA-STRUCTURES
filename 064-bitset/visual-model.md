# Visual Model — 70-bit Bitset

    word 0: bits 0..63
    word 1: bits 64..69 + 58 padding bits

Index 65:

    word = 65 / 64 = 1
    offset = 1

    mask = 1ULL << 1
