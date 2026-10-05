# Implementation — ByteSuffixTree

Owned symbols:
- original bytes widened to uint16_t
- terminal symbol 256 appended

Node arena:
- dynamic edge array
- suffix_start for leaves
- cached leaf_count

Edge:
- start/end indices into symbols
- child node index

All structural references are indices, not pointers into the reallocating node arena.
