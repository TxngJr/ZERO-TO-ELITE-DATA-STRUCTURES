# Complexity — Trie

Let:
- L = input length
- d_i = scanned degree at path node i
- sigma=256 fixed byte alphabet

Path lookup exact cost:

    O(sum d_i)

Worst:
    O(L*sigma)

Since sigma is fixed:
    O(L)

count-prefix additionally scans matching subtree.

Lexicographic output necessarily costs at least proportional to returned keys/bytes.
