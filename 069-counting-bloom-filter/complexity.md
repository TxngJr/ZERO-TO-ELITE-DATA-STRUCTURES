# Complexity — Counting Bloom Filter

Let k be hash positions:
- add: O(k)
- remove: O(k)
- maybe_contains: O(k)
- validation: O(m)
- storage: Theta(m) counters

Using 16-bit counters costs about 16x the raw bit storage of a plain Bloom bit array before metadata.
