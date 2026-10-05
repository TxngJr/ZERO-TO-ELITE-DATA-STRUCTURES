# Complexity — XOR Trie

Fixed-width uint64_t has 64 levels.

- insert/remove/contains: O(64)
- min/max XOR: O(64)
- count XOR < limit: O(64)

General w-bit model: O(w).

Storage depends on allocated prefix nodes; this implementation does not recycle dead nodes after deletion.
