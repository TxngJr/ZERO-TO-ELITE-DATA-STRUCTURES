# Complexity — Radix Tree

Let L be key length.

Along one root-to-key path total label bytes compared are O(L).

With fixed byte alphabet:
- insert O(L)
- contains O(L)
- has-prefix O(L)
- remove O(L) plus affected label-copy work
- count-prefix O(L + subtree)
- visit-prefix O(L + output)

Storage can be much smaller than ordinary Trie when long unary paths exist.
