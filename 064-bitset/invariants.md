# Invariants — IntBitset

1. word_count == ceil(bit_count/64)
2. bit_count==0 implies word_count==0
3. word_count>0 implies words!=NULL
4. every logical bit maps to exactly one word/mask
5. padding bits above bit_count are always zero
6. bitwise operations require equal lengths
