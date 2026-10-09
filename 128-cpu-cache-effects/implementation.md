# Implementation Notes

Each set owns a fixed-length tag segment and a valid-count.

Within the valid prefix:
- index 0 = MRU
- index valid_count-1 = LRU

On hit, the matching tag is shifted to index 0.
On miss with room, valid tags shift right and new tag becomes MRU.
On miss when full, the last tag is discarded and `evictions` increments.

This representation avoids timestamp overflow and makes LRU invariants explicit.

Configuration arithmetic checks:
- line_size > 0
- set_count > 0
- associativity > 0
- set_count × associativity allocation overflow
