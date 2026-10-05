# Theory — XOR Trie

Binary trie groups integers by bit prefixes.

XOR optimization depends on lexicographic significance of bits: first differing XOR bit from MSB decides which score is larger.

Therefore greedy traversal is correct:
- maximize -> choose opposite query bit when live
- minimize -> choose same query bit when live

subtree_count ทำให้ threshold query เพิ่มทั้ง accepted prefix subtree ได้ใน O(1) ต่อ level.
