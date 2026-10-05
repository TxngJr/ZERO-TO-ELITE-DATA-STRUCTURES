# Complexity — Bloom Filter

For k hash probes:
- add: O(k)
- maybe_contains: O(k)
- set-bit count: O(m/64)
- storage: Theta(m) bits

False-positive probability depends on m, k, inserted-key distribution and hash quality.
