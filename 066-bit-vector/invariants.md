# Invariants — IntBitVector

1. word_count == ceil(bit_count/64)
2. padding bits are zero
3. prefix_ones has word_count+1 entries
4. prefix_ones[0] == 0
5. prefix_ones is nondecreasing
6. prefix_ones[w+1]-prefix_ones[w] == popcount(words[w])
7. prefix_ones[word_count] == total ones
8. rank0(end)+rank1(end) == end
9. select never returns padding positions
