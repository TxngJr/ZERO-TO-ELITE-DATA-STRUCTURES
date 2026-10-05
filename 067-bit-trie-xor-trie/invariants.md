# Invariants — IntXorTrie

1. root index is 0
2. missing child is SIZE_MAX
3. every present child index is valid
4. nodes above depth 64 have terminal_count == 0
5. leaf depth 64 has no children
6. leaf subtree_count == terminal_count
7. internal subtree_count equals sum of child subtree counts
8. root subtree_count equals trie size
9. zero-count historical nodes may remain reachable but queries ignore them
