# Complexity — Bitset

Let W = ceil(n/64).

- test/set/clear/flip: O(1)
- clear_all/set_all/flip_all: O(W)
- count: O(W)
- any: O(W) worst case
- all: O(W)
- AND/OR/XOR/NOT: O(W)
- find_next_set: O(W) worst case
- storage: Theta(W)
