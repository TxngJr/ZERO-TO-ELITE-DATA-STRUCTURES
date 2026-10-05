# Implementation — ByteSuffixArray

Owned state:
- text copy
- suffix_array[]
- inverse_rank[]
- lcp[]

Build:
1. initial byte ranks
2. prefix-doubling RankPair qsort
3. inverse SA rank
4. Kasai LCP

Search compares a suffix directly against a byte pattern without allocating substrings.
