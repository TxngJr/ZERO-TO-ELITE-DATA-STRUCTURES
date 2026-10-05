# Invariants — ByteSuffixArray

1. SA is a permutation of 0..n-1
2. suffixes are strictly lexicographically sorted
3. inverse_rank[SA[r]] == r
4. LCP[0] == 0
5. LCP[r] equals exact common-prefix length of adjacent suffixes
6. search-match range is contiguous in SA order
