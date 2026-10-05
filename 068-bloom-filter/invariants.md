# Invariants — ByteBloomFilter

1. bit_count > 0
2. hash_count > 0
3. word_count == ceil(bit_count/64)
4. every generated index is in [0,bit_count)
5. padding bits in final word are zero
6. add never clears bits
7. inserted keys therefore cannot become negative without external mutation
