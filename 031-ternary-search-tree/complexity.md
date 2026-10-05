# Complexity — TST

Let L be key length.

If alternative-symbol search at each depth is balanced:
    roughly O(L log sigma)

For fixed byte alphabet sigma<=256 this is effectively O(L) with constants.

Worst-case skewed lo/high chains can make a lookup much worse than direct Trie child indexing.

Traversal:
    Theta(number of nodes + output bytes)

Storage:
    Theta(number of TST nodes)

Each node uses exactly three child pointers.
