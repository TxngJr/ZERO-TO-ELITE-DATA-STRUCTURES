# Invariants — ByteSuffixTree

1. symbol array ends with unique sentinel 256
2. all edge labels are nonempty valid symbol intervals
3. outgoing edges from one node have distinct first symbols
4. every child node has exactly one incoming tree edge
5. every suffix 0..n is represented by exactly one leaf
6. leaf suffix_start is unique
7. internal/root nodes use suffix_start=SIZE_MAX
8. path from root to leaf s spells symbols[s..n+1)
9. leaf_count equals number of descendant suffix leaves
10. reachable node count equals arena node_count
