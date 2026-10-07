# Implementation Notes

Route trie stores children as node indices rather than raw pointers, so dynamic `realloc` growth cannot invalidate child references.

Route removal clears route metadata but intentionally does not prune dead path nodes; this keeps removal simple and node indices stable.

Flow-table rehash allocates the new table first and reinserts only live entries. Tombstones disappear during rehash.

Flow-key equality compares fields explicitly instead of `memcmp`, avoiding struct-padding dependence.
