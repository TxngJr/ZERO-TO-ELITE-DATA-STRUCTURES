# Invariants — ByteTrie

1. root always exists while trie exists
2. edges of each node strictly increase by unsigned-byte label
3. edge child is non-NULL
4. one child at most per label
5. size equals number of terminal nodes
6. each represented key corresponds to one root path
7. deletion never removes a node needed by another key
