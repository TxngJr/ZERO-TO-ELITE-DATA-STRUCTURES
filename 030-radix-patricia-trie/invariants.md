# Invariants — ByteRadixTree

1. root exists
2. every edge label length > 0
3. label pointer non-NULL
4. edge child non-NULL
5. siblings sorted by first byte
6. sibling first bytes unique
7. terminal count equals size
8. each key corresponds to one concatenated root-to-terminal edge path

Maximal compression is desirable but not required for semantic validity after optional recompression allocation failure.
