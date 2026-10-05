# Implementation

Both use sorted dynamic arrays. Multiset compresses repeated keys. Multimap uses lower_bound/upper_bound so all equal-key pairs are contiguous and equal-key insertion order is stable.
